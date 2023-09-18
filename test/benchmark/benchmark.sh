#!/bin/bash
if [ -z "${1+:}" ]; then
    printf "Usage: %s [DBNAME]\n" "$0"
    exit 1
fi

psql --username="$1" --dbname="$1" --command="CREATE EXTENSION IF NOT EXISTS pg_uuidv7;"
pgbench --username="$1" --initialize "$1"

FILES=$(ls -- *.sql)
for FILE in $FILES; do
    printf "Benchmarking %s\n" "$FILE"
    for _ in {0..5}; do
        pgbench --client=8 --jobs=8 --transactions=200000 --file="$FILE" --username="$1" "$1"
        sleep 1
    done
    printf "\n\n"
done
