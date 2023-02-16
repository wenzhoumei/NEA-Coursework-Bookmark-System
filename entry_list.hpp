#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <set>
#include "entry.hpp"
#include "entry_processor.hpp"

class EntryList {
public:
    // Return false if any of these fail else return true
    virtual bool RemoveEntry() = 0;
    virtual bool AddEntry(std::unique_ptr<Entry> entry) = 0;
    virtual bool UpdateEntry(std::unique_ptr<Entry> entry) = 0;
            
    virtual void Up() = 0;
    virtual void Down() = 0;

    virtual size_t Size() const = 0;

    virtual void PrintEntries() const = 0;
};

class VectorEntryList: public EntryList {
protected:
    std::vector<std::unique_ptr<Entry>> entries_;

    size_t selected = 0;

    bool Exists_(const std::unique_ptr<Entry>& entry) {
        for (const std::unique_ptr<Entry>& e : entries_) {
            if (entry->GetString() == e->GetString()) {
		return true;
            }
        }

        return false;
    }

public:
    void PrintEntries() const override {
	for (const auto& entry: entries_) {
	    std::cout << entry->GetString() << std::endl;
	}
    }

    size_t Size() const override {
	return entries_.size();
    }

    virtual bool RemoveEntry() override {
        if (selected >= Size()) {
            std::cerr << "Error: Index out of bounds" << std::endl;
            return false;
        }

        entries_.erase(entries_.begin() + selected);
	return true;
    }

    virtual bool AddEntry(std::unique_ptr<Entry> entry) override {
        if (Exists_(entry)) {
            std::cerr << "Error: Entry already exists" << std::endl;
            return false;
        }

        entries_.push_back(std::move(entry));
	return true;
    }

    virtual bool UpdateEntry(std::unique_ptr<Entry> entry) override {
        if (selected >= Size()) {
            std::cerr << "Error: Index out of bounds" << std::endl;
            return false;
        }

        if (Exists_(entry)) {
            std::cerr << "Error: Entry already exists" << std::endl;
            return false;
        }

        entries_[selected] = std::move(entry);
	return true;
    }

    void Up() override {
	if (selected > 0) {
	    selected--;
	}
    }

    void Down() override {
	if (selected < entries_.size() - 1) {
	    selected++;
	}
    }
};

class FileEntryList: public VectorEntryList {
protected:
    std::filesystem::path file_path_;

    bool WriteToFile_() {
        std::ofstream file(file_path_);

        if (!file) {
            std::cerr << "Error: Unable to open file for writing: " << file_path_ << std::endl;
            return false;
        }

        for (const auto &entry : entries_) {
            file << entry->GetString() << std::endl;
        }

        return true;
    }

public:
    FileEntryList(std::filesystem::path file_path) 
	: file_path_(file_path)
    {
        std::ifstream file(file_path_);

	if (!file) {
	    // File doesn't exist, so create it
	    std::ofstream new_file(file_path_);
	    if (!new_file) {
		std::cerr << "Error: Unable to create file: " << file_path_ << std::endl;
		return;
	    }
	} else {
	    // File exists, so read entries from it
	    std::string line;
	    while (std::getline(file, line)) {
		entries_.push_back(EntryProcessor::CreateEntry(line));
	    }
	}
    }

    bool RemoveEntry() override {
        return VectorEntryList::RemoveEntry() && WriteToFile_();
    }

    bool AddEntry(std::unique_ptr<Entry> entry) override {
        return VectorEntryList::AddEntry(std::move(entry)) && WriteToFile_();
    }

    bool UpdateEntry(std::unique_ptr<Entry> entry) override {
        return VectorEntryList::UpdateEntry(std::move(entry)) && WriteToFile_();
    }
};

struct EntryComparator {
    bool operator()(const std::unique_ptr<Entry>& a, const std::unique_ptr<Entry>& b) const {
        return a->GetString() < b->GetString();
    }
};

class SetEntryList : public EntryList {
protected:
    std::set<std::unique_ptr<Entry>, EntryComparator> entries_;
    std::set<std::unique_ptr<Entry>, EntryComparator>::iterator selected_;

    bool Exists_(const std::unique_ptr<Entry>& entry) {
        return entries_.contains(entry);
    }

public:
    void PrintEntries() const override {
        for (const auto& entry : entries_) {
            std::cout << entry->GetString() << std::endl;
        }
    }

    size_t Size() const override {
        return entries_.size();
    }

    virtual bool RemoveEntry() override {
        if (selected_ == entries_.end()) {
            std::cerr << "Error: No entry selected" << std::endl;
            return false;
        }

        entries_.erase(selected_);
        selected_ = entries_.end();
        return true;
    }

    virtual bool AddEntry(std::unique_ptr<Entry> entry) override {
        if (Exists_(entry)) {
            std::cerr << "Error: Entry already exists" << std::endl;
            return false;
        }

        entries_.emplace(std::move(entry));
        return true;
    }

    virtual bool UpdateEntry(std::unique_ptr<Entry> entry) override {
        if (selected_ == entries_.end()) {
            std::cerr << "Error: No entry selected" << std::endl;
            return false;
        }

        if (Exists_(entry)) {
            std::cerr << "Error: Entry already exists" << std::endl;
            return false;
        }

        entries_.erase(selected_);
        selected_ = entries_.emplace(std::move(entry)).first;
        return true;
    }

    void Up() override {
        if (selected_ != entries_.begin()) {
            --selected_;
        }
    }

    void Down() override {
        if (selected_ != entries_.end()) {
            ++selected_;
        }
    }
};

class DirectoryEntryList : public SetEntryList {
public:
    std::filesystem::path directory_path;

    DirectoryEntryList(std::filesystem::path directory_path) : directory_path(directory_path) {
        // Load entries from directory
        for (const auto &entry : std::filesystem::directory_iterator(directory_path)) {
            if (entry.is_regular_file()) {
                entries_.emplace(EntryProcessor::CreateEntry(entry.path().filename()));
            }
        }
    }

    bool RemoveEntry() override {

        // Remove the corresponding file in the directory
        std::filesystem::remove(directory_path / std::filesystem::path((*selected_)->GetString()));

        return SetEntryList::RemoveEntry();
    }

    bool UpdateEntry(std::unique_ptr<Entry> entry) override {
	int result = std::rename((directory_path / (*selected_)->GetString()).c_str(), (directory_path / entry->GetString()).c_str());

	bool success = false;
	if (result == 0) {
	    success = true;
	}

	return success && SetEntryList::UpdateEntry(std::move(entry));
    }

    bool AddEntry(std::unique_ptr<Entry> entry) override {
        // Create the corresponding file in the directory
	std::ofstream file(directory_path / entry->GetString());
        file.close();

        return SetEntryList::AddEntry(std::move(entry));
    }

};

class ExecutableDirectoryEntryList : public DirectoryEntryList {
public:
    ExecutableDirectoryEntryList(std::filesystem::path directory_path): DirectoryEntryList(directory_path) {}

    bool AddEntry(std::unique_ptr<Entry> entry) override {
	bool ret = DirectoryEntryList::AddEntry(std::move(entry));

	// Make file executable
	std::system(("chmod +x " + (directory_path / entry->GetString()).string()).c_str());

        return ret;
    }
};

/*

class SetEntryList : public EntryList {
protected:
    std::set<Entry> entries_;

public:
    SetDirectoryList(int& selected): EntryList(selected) {};

    void PrintEntries() const override {
	for (Entry const& entry: entries_) {
	    std::cout << entry.GetString() << " ";
	}
    }

    bool RemoveEntry(unsigned int i) override {
        // Check if the entry index is within bounds
        if (i >= entries.size()) return false;

        // Remove the entry from the entries vector
        entries.erase(entries.begin() + i);

        // Remove the corresponding file in the directory
        std::filesystem::remove(directory_path / std::filesystem::path(entries[i].GetString()));

        return true;
    }

    bool UpdateEntry(unsigned int i, Entry entry) override {
        if (i >= entries.size()) {
            std::cerr << "Error: Index out of bounds" << std::endl;
            return false;
        }

        if (Exists_(entry)) {
            std::cerr << "Error: Entry already exists" << std::endl;
            return false;
        }

	int result = std::rename((directory_path / entries[i].GetString()).c_str(), (directory_path / entry.GetString()).c_str());

	if (result == 0) {
	    return true;
	} else {
	    return false;
	}
    }

    bool AddEntry(Entry entry) override {
        // Check if the entry already exists in the entries vector
	if (Exists_(entry)) {
            std::cerr << "Error: Entry already exists" << std::endl;
	    return false;
	}

	// Insert the entry in the sorted order
        auto it = std::lower_bound(entries.begin(), entries.end(), entry);
        entries.insert(it, entry);

        // Create the corresponding file in the directory
	std::ofstream file(directory_path / entry.GetString());
        file.close();

        return true;
    }
};
class DirectoryEntryList : public EntryList {
public:
    std::filesystem::path directory_path;

    DirectoryEntryList(std::filesystem::path directory_path) : directory_path(directory_path) {
        // Load entries from directory
        for (const auto &entry : std::filesystem::directory_iterator(directory_path)) {
            if (entry.is_regular_file()) {
                entries.push_back(Entry(entry.path().filename()));
            }
        }
    }

    bool RemoveEntry(unsigned int i) override {
        // Check if the entry index is within bounds
        if (i >= entries.size()) return false;

        // Remove the entry from the entries vector
        entries.erase(entries.begin() + i);

        // Remove the corresponding file in the directory
        std::filesystem::remove(directory_path / std::filesystem::path(entries[i].GetString()));

        return true;
    }

    bool UpdateEntry(unsigned int i, Entry entry) override {
        if (i >= entries.size()) {
            std::cerr << "Error: Index out of bounds" << std::endl;
            return false;
        }

        if (Exists_(entry)) {
            std::cerr << "Error: Entry already exists" << std::endl;
            return false;
        }

	int result = std::rename((directory_path / entries[i].GetString()).c_str(), (directory_path / entry.GetString()).c_str());

	if (result == 0) {
	    return true;
	} else {
	    return false;
	}
    }

    bool AddEntry(Entry entry) override {
        // Check if the entry already exists in the entries vector
	if (Exists_(entry)) {
            std::cerr << "Error: Entry already exists" << std::endl;
	    return false;
	}

	// Insert the entry in the sorted order
        auto it = std::lower_bound(entries.begin(), entries.end(), entry);
        entries.insert(it, entry);

        // Create the corresponding file in the directory
	std::ofstream file(directory_path / entry.GetString());
        file.close();

        return true;
    }

};

class ExecutableDirectoryEntryList : public DirectoryEntryList {
public:
    ExecutableDirectoryEntryList(std::filesystem::path directory_path)
	: DirectoryEntryList(directory_path)
    {
    }

    bool AddEntry(Entry entry) override {
        // Check if the entry already exists in the entries vector
	if (Exists_(entry)) {
            std::cerr << "Error: Entry already exists" << std::endl;
	    return false;
	}

	// Insert the entry in the sorted order
        auto it = std::lower_bound(entries.begin(), entries.end(), entry);
        entries.insert(it, entry);

	std::filesystem::path file_path = directory_path / entry.GetString();

        // Create the corresponding file in the directory
	std::ofstream file(file_path);
        file.close();

	// Make file executable
	std::system(("chmod +x " + file_path.string()).c_str());

        return true;
    }
};

class CompositeEntryList EntryList {
};
*/
