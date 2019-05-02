#!/bin/bash

try() {
  expected="$1"
  input="$2"

  ./zcc "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$expected expected, but got $actual"
    exit 1
  fi
}

try 0 "0"
try 123 "123"
try 3 '1+2'
try 0 '11+22-33'

echo OK
