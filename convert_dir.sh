#!/bin/bash

# Function to convert CamelCase to snake_case for directory names
camel_to_snake_dir() {
    echo "$1" | sed 's/\([a-z]\)\([A-Z]\)/\1_\2/g' | tr '[:upper:]' '[:lower:]'
}

# Function to convert PascalCase to snake_case for directory names
pascal_to_snake_dir() {
    echo "$1" | sed 's/\([a-z]\)\([A-Z]\)/\1_\2/g' | sed 's/./\L&/g'
}

# Check if at least one argument is provided
if [ "$#" -eq 0 ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

directory="$1"

# Check if the directory exists
if [ ! -d "$directory" ]; then
    echo "Error: Directory '$directory' not found."
    exit 1
fi

# Recursively process each folder in the directory
find "$directory" -type d -exec bash -c '
for dir in "$@"; do
    # Extract the folder name
    base=$(basename "$dir")
    parent_dir=$(dirname "$dir")

    # Convert folder name to snake_case
    snake_case=$(camel_to_snake_dir "$base")
    snake_case=$(pascal_to_snake_dir "$snake_case")

    # Rename the folder
    mv "$dir" "$parent_dir/$snake_case"
    echo "Renamed: $dir -> $parent_dir/$snake_case"
done
' bash {} +

echo "Folder renaming complete in directory '$directory'."
