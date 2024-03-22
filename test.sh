#!/bin/bash

# . scripts/test.sh

test="test1"
gcc -o "custom_tests/c90_tests/${test}" -std=c90 -ansi "custom_tests/c90_tests/${test}.c"
./custom_tests/c90_tests/${test}
