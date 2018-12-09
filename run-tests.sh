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

"${GENERATOR}" -s "${STUDENT_ID}" -r | "${BIN_FILE}" "${OUTPUT_DIR}"/standard-uniform.res
"${GENERATOR}" -s "${STUDENT_ID}" -b | "${BIN_FILE}" "${OUTPUT_DIR}"/standard-imbalanced.res
"${GENERATOR}" -s "${STUDENT_ID}" -x | "${BIN_FILE}" "${OUTPUT_DIR}"/standard-cunning.res
"${GENERATOR}" -s "${STUDENT_ID}" -p | "${BIN_FILE}" "${OUTPUT_DIR}"/standard-deep.res

make clean
make ARGS=-DNAIVE_FH

"${GENERATOR}" -s "${STUDENT_ID}" -r | "${BIN_FILE}" "${OUTPUT_DIR}"/naive-uniform.res
"${GENERATOR}" -s "${STUDENT_ID}" -b | "${BIN_FILE}" "${OUTPUT_DIR}"/naive-imbalanced.res
"${GENERATOR}" -s "${STUDENT_ID}" -x | "${BIN_FILE}" "${OUTPUT_DIR}"/naive-cunning.res
"${GENERATOR}" -s "${STUDENT_ID}" -p | "${BIN_FILE}" "${OUTPUT_DIR}"/naive-deep.res

