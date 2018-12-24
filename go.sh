#!/bin/bash

while true; do
  ./judge.out operations.in
  if [ $? -ne 0 ]; then
    echo "Here it is you stupid bitch."
    break
  fi
  rm operations.in
done
