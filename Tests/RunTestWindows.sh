#!/bin/bash
# Name test files in the following manner: <testName>_queries.txt <testName>_source.txt
if [ -z "$1" ]
  then
    echo "Running all system tests..."
    for sourceFileName in $(ls system_tests | egrep -i 'source.txt' ); do
      testName=(${sourceFileName//_/ })
      ../Code09/out/build/x64-Debug/src/autotester/autotester.exe system_tests/${testName}_source.txt system_tests/${testName}_queries.txt out/${testName}_output.xml
    done
    exit 0
else
  echo "Running targeted test"
  ../Code09/out/build/x64-Debug/src/autotester/autotester.exe ${1}_source.txt ${1}_queries.txt out/${1}_output.xml
  exit 0
fi