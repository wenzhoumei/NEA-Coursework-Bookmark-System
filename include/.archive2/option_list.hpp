#pragma once
#include <unordered_map>
#include <string>
#include <vector>

/**
 * @class OptionList
 * @brief A class that represents a list of options.
 * Option is an entry of a menu
 */
class OptionList {
public:
    /**
     * @brief Loads options from a file or directory.
     * @return true if options were loaded successfully, false otherwise.
     */
    virtual bool LoadOptions() = 0;

    /**
     * @brief Checks if an option with the given name exists in the list.
     * @param option_name The name of the option to check.
     * @return true if the option exists, false otherwise.
     */
    bool ContainsOption(const std::string& option_name) const;

    /**
     * @brief Returns a sublist of options from the specified start index to the specified end index.
     * @param start The start index of the sublist.
     * @param end The end index of the sublist.
     * @return A vector containing the sublist of options.
     */
    std::vector<std::string> SubList(size_t start, size_t end) const;

    /**
     * @brief Returns the data of the option with the given name.
     * @param option_name The name of the option to retrieve the data for.
     * @return The data of the option if it exists, an empty string otherwise.
     */
    std::string DataAt(std::string option_name) const;

    /**
     * @brief Returns the number of options in the list.
     * @return The number of options in the list.
     */
    size_t Size() const;

    /**
     * @brief Removes an option with the given name from the list.
     * @param option_name The name of the option to remove.
     * @return true if the option was removed successfully, false otherwise.
     */
    virtual bool RemoveOption(const std::string& option_name);

    /**
     * @brief Adds a new option with the given name and data to the list.
     * @param option_name The name of the option to add.
     * @param option_data The data of the option to add.
     * @return true if the option was added successfully, false otherwise.
     */
    virtual bool AddOption(const std::string& option_name, const std::string& option_data);

    /**
     * @brief Inserts a new option with the given name and data at the specified position in the list.
     * @param pos The position to insert the option at.
     * @param option_name The name of the option to insert.
     * @param option_data The data of the option to insert.
     * @return true if the option was inserted successfully, false otherwise.
     */
    virtual bool InsertOption(size_t pos, const std::string& option_name, const std::string& option_data);

    /**
     * @brief Updates the data of an option with the given name in the list. If the option does not exist, it is added to the end of the list.
     *        If the option already exists in the list, its position is swapped with the option that comes after it.
     * @param option_name The name of the option to update.
     * @param option_data The new data of the option.
     * @return true if the option was updated or added successfully, false otherwise.
     */
    virtual bool UpdateOption(const std::string& option_name, const std::string& option_data);

protected:
    std::unordered_map<std::string, std::string> names_to_data_; /**< A mapping of option names to their data */
    std::vector<std::string> option_order_; /**< A vector that stores the order of options */
};
