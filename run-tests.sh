#!/bin/bash

STUDENT_ID=64
GENERATOR=./generator
OUTPUT_DIR=outputs
BIN_FILE=./tester.out

mkdir "${OUTPUT_DIR}"
make clean

make generator
if [ $? -ne 0 ]; then
  echo "Couldn't compile the generator."
  exit 1
fi

make 
if [ $? -ne 0 ]; then
  echo "Couldn't compile standard Fibonacci heap."
  exit 2
fi

set -o pipefail

echo "Running standard uniform test."
"${GENERATOR}" -s "${STUDENT_ID}" -r | valgrind "${BIN_FILE}" "${OUTPUT_DIR}"/standard-uniform.res
if [ $? -ne 0 ]; then
  echo "Standard uniform test failed."
  exit 3
fi
echo "Running standard imbalanced test."
"${GENERATOR}" -s "${STUDENT_ID}" -b | "${BIN_FILE}" "${OUTPUT_DIR}"/standard-imbalanced.res
if [ $? -ne 0 ]; then
  echo "Standard imbalanced test failed."
  exit 3
fi
echo "Running standard cunning test."
"${GENERATOR}" -s "${STUDENT_ID}" -x | valgrind "${BIN_FILE}" "${OUTPUT_DIR}"/standard-cunning.res
if [ $? -ne 0 ]; then
  echo "Standard cunning test failed."
  exit 3
fi
echo "Running standard deep test."
"${GENERATOR}" -s "${STUDENT_ID}" -p | valgrind "${BIN_FILE}" "${OUTPUT_DIR}"/standard-deep.res
if [ $? -ne 0 ]; then
  echo "Standard deep test failed."
  exit 3
fi

make clean
make ARGS=-DNAIVE_FH

echo "Running naive uniform test."
"${GENERATOR}" -s "${STUDENT_ID}" -r | "${BIN_FILE}" "${OUTPUT_DIR}"/naive-uniform.res
if [ $? -ne 0 ]; then
  echo "Naive uniform test failed."
  exit 4
fi
echo "Running naive imbalanced test."
"${GENERATOR}" -s "${STUDENT_ID}" -b | "${BIN_FILE}" "${OUTPUT_DIR}"/naive-imbalanced.res
if [ $? -ne 0 ]; then
  echo "Naive imbalaned test failed."
  exit 4
fi
echo "Running naive cunning test."
"${GENERATOR}" -s "${STUDENT_ID}" -x | "${BIN_FILE}" "${OUTPUT_DIR}"/naive-cunning.res
if [ $? -ne 0 ]; then
  echo "Naive cunning test failed."
  exit 4
fi
echo "Running naive deep test."
"${GENERATOR}" -s "${STUDENT_ID}" -p | "${BIN_FILE}" "${OUTPUT_DIR}"/naive-deep.res
if [ $? -ne 0 ]; then
  echo "Naive deep test failed."
  exit 4
fi

