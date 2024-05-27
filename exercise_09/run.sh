#!/bin/bash

# Define the possible values for mapper and sel
mappers=("circle" "square" "provita")
sels=(0 1 2 3)

# Loop through all combinations of mapper and sel
for mapper in "${mappers[@]}"; do
    for sel in "${sels[@]}"; do
        # Print a message indicating the current mapper and sel values
        echo "Running exercise_09 with mapper: $mapper and sel: $sel"
        
        # Call ./exercise_09 with the current mapper and sel values
        ../build/exercise_09/exercise_09 "$mapper" "$sel"
    done
done