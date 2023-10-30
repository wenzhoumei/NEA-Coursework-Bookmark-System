#!/bin/bash

if [ -d "$HOME/.config/my_menu" ]; then
    echo "Directory exists. Removing it..."
    rm -r "$HOME/.config/my_menu"
    echo "Directory removed."
else
    echo "Directory does not exist."
fi
