# Clear all databases and their names

#!/bin/bash

cd "$(dirname "$0")/.."

DATABASES_DIR="./databases"

if [ -d "$DATABASES_DIR" ]; then
    chmod -R u+w "$DATABASES_DIR"
    chmod -R u+x "$DATABASES_DIR"
    rm -rf "$DATABASES_DIR"
fi

mkdir -p "$DATABASES_DIR"
if [ $? -eq 0 ]; then
    chmod u+w "$DATABASES_DIR"
else
    exit 1
fi

touch "$DATABASES_DIR/db_names.txt"
if [ $? -eq 0 ]; then
    chmod u+w "$DATABASES_DIR/db_names.txt"
else
    exit 1
fi

# echo "Databases cleared successfully"

exit 0