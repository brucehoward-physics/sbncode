#!/bin/bash
for filename in *.xml 
do

echo "running project.py on: $filename"
#project.py --xml $filename --stage gen --checkana
project.py --xml $filename --stage g4 --submit
done
