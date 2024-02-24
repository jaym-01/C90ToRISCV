set -e
test="test"

# Compile to assembly
# make bin/c_compiler
# ./bin/c_compiler -S "$test.c" -o "bin/riscv_example.s"

# Compile and run test
make bin/c_compiler
rm -f bin/riscv_example.s
./bin/c_compiler -S "$test.c" -o "bin/riscv_example.s"
# riscv64-unknown-elf-gcc -march=rv32imfd -mabi=ilp32d -o "bin/riscv_example" "bin/riscv_example.s" "${test}_driver.c"
riscv64-unknown-elf-gcc -o "test" "bin/riscv_example.s" "${test}_driver.c"
set +e
spike pk "test"
rm test

# Run assembly test
# riscv64-unknown-elf-gcc -o "test" "bin/riscv_example.s" "test_driver.c"
# spike pk "test"




# EXTRA
# set +e
# spike pk "bin/riscv_example"

# rm -f bin/riscv_example.s
