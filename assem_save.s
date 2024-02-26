.text
.globl f
f:
addi sp, sp, -20
sw ra, 16(sp)
sw fp, 12(sp)
addi fp, sp, 20
sw a0, -20(fp) # store int n
li t6, 0 # load 0
lw t5, -20(fp) # load n
sub t4, t5, t6
seqz t4, t4 # eval n == 0
beq t4, zero, endif_label_1 # Jump to endif_label_1 if n != 0
li a0, 0
j return_0
j endif_label_1
endif_label_1: # return n + f(n - 1)
li t4, 1
lw t6, -20(fp)
sub a0, t6, t4 # argument: n - 1
call f
add t4, a0, zero # store f(n - 1)
lw t6, -20(fp) # load n
add a0, t6, t4 # return n + f(n - 1)
j return_0
return_0: # function return
lw ra, 16(sp)
lw fp, 12(sp)
addi sp, sp, 20
jr ra

.text
.globl test1
test1:
addi sp, sp, -16
sw ra, 12(sp)
sw fp, 8(sp)
addi fp, sp, 16
li a0, 5
call f
add a0, a0, zero
j return_2
return_2:
lw ra, 12(sp)
lw fp, 8(sp)
addi sp, sp, 16
jr ra

