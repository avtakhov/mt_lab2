#!/bin/bash
g++ -std=c++17 main.cpp tokenizer.cpp tree.cpp tokenizer.h tree.h -o mt_lab2.out
a=1

for i in "$@"
do
  testdir="./test$a"
  mkdir "$testdir"
  echo "$i" > "$testdir/input.txt"
  ./mt_lab2.out "$i" "$testdir/error.txt" | /usr/bin/dot -Tsvg > "$testdir/output.svg"
  a=$((a+1))
done