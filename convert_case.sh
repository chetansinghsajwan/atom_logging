#!/bin/bash

# Function to convert CamelCase to snake_case
camel_to_snake() {
    echo "$1" | sed 's/\([a-z]\)\([A-Z]\)/\1_\2/g' | tr '[:upper:]' '[:lower:]'
}

# Function to convert PascalCase to snake_case
pascal_to_snake() {
    echo "$1" | sed 's/\([a-z]\)\([A-Z]\)/\1_\2/g' | sed 's/./\L&/g'
}

# Check if a file is provided as an argument
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <file_containing_list_of_files>"
    exit 1
fi

file_list="$1"

# Check if the file exists
if [ ! -f "$file_list" ]; then
    echo "Error: File '$file_list' not found."
    exit 1
fi

# Process each file listed in the file
while IFS= read -r filename; do
    # Check if the file exists
    if [ ! -f "$filename" ]; then
        echo "Error: File '$filename' not found."
        continue
    fi

    # Process each line in the file
    while IFS= read -r line; do
        snake_case=$(camel_to_snake "$line")
        snake_case=$(pascal_to_snake "$snake_case")
        echo "$snake_case"
    done < "$filename" > "$filename.tmp"

    # Replace the original file with the modified one
    mv "$filename.tmp" "$filename"

    echo "Conversion complete. Check the updated content in '$filename'."
done < "$file_list"
