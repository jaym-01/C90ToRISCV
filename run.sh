set -e
test="test6"

# Compile to assembly
# make bin/c_compiler
# ./bin/c_compiler -S "$test.c" -o "bin/riscv_example.s"

# Compile and run test
make bin/c_compiler
rm -f "custom_tests/${test}/out/${test}.s"
./bin/c_compiler -S "custom_tests/${test}/in/test.c" -o "custom_tests/${test}/out/test.s"
riscv64-unknown-elf-gcc -march=rv32imfd -mabi=ilp32d -o "test" "custom_tests/${test}/out/test.s" "custom_tests/${test}/in/test_driver.c"
# riscv64-unknown-elf-gcc -o "test" "bin/riscv_example.s" "${test}_driver.c"
spike pk "test"
rm test

# Run assembly test
# # riscv64-unknown-elf-gcc -march=rv32imfd -mabi=ilp32d -o "test" "bin/riscv_example.s" "test_driver.c"
# riscv64-unknown-elf-gcc -o "test" "bin/riscv_example.s" "test_driver.c"
# spike pk "test"
# rm test



# EXTRA
# set +e
# spike pk "bin/riscv_example"

# rm -f bin/riscv_example.s
