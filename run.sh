# "compiler_tests/_example/example.c"


test="test.c"
make bin/c_compiler
# rm -f bin/riscv_example.s
./bin/c_compiler -S $test -o "bin/riscv_example.s"


# make bin/c_compiler
# rm -f bin/riscv_example.s
# ./bin/c_compiler -S "compiler_tests/_example/example.c" -o "bin/riscv_example.s"
# riscv64-unknown-elf-gcc -march=rv32imfd -mabi=ilp32d -o "bin/riscv_example" "bin/riscv_example.s" "compiler_tests/_example/example_driver.c"

# set +e
# spike pk "bin/riscv_example"

# rm -f bin/riscv_example.s
