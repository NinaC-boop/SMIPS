# Read 2 numbers and use them as indices into a 2d-array
# x in $t0, y in $t1
main:
    la $a0, prompt_x    # printf("Enter x: ");
    li $v0, 4
    syscall

    li $v0, 5           # scanf("%d", &x);
    syscall             #
    move $t0, $v0

    la $a0, prompt_y    # printf("Enter y: ");
    li $v0, 4
    syscall

    li $v0, 5           # scanf("%d", &y);
    syscall             #
    move $t1, $v0


    # PUT YOUR CODE HERE
    mul $t0, $t0, 24    # offset = row * 24
    mul $t0, $t0, 4     
    mul $t1, $t1, 4
    add $t0, $t0, $t1   # offset = row * 24 + col

	lw $a0, array($t0)
	li $v0, 1
	syscall				# printf("%d", offset);

    li   $a0, '\n'      # printf("%c", '\n');
    li   $v0, 11
    syscall

    li $v0, 0           # return 0
    jr $31

.data
prompt_x:
    .asciiz "Enter x: "
prompt_y:
    .asciiz "Enter y: "
array:
    .word 9 4 3 2 5 1 1 4 3 1 2 6 7 5 6 2 8 1 8 3 4 1 1 1
    .word 7 3 9 6 6 2 4 8 6 8 1 9 8 2 9 5 9 8 9 9 2 3 1 1
    .word 1 4 6 5 4 2 9 5 7 9 5 6 4 1 6 9 6 1 9 3 8 3 1 2
    .word 3 3 3 9 7 3 7 1 3 2 3 7 7 3 2 5 8 1 4 2 4 5 9 4
    .word 5 7 6 9 4 2 4 9 4 7 9 2 8 1 6 2 7 4 9 7 1 9 3 5
    .word 8 3 8 9 3 4 6 2 4 1 3 3 2 1 2 4 2 7 8 6 8 6 9 9
    .word 9 5 8 9 7 5 6 6 2 9 5 1 1 8 6 8 3 4 1 1 5 9 5 2
    .word 3 2 4 1 4 8 2 8 7 6 7 8 8 3 8 2 6 5 5 5 5 9 5 3
    .word 7 1 3 9 8 8 6 3 1 7 6 5 6 9 3 8 1 5 7 6 7 7 5 6
    .word 4 6 5 7 4 1 4 7 3 5 5 7 9 6 8 4 3 1 9 9 2 6 8 9
    .word 2 3 8 5 8 8 7 1 8 1 1 8 2 2 3 9 7 6 7 9 3 2 6 5
    .word 1 4 7 4 7 7 7 7 9 9 8 9 5 5 3 3 9 5 8 7 7 6 1 7
    .word 5 3 8 7 5 6 1 9 5 6 3 3 5 9 9 5 4 1 3 8 1 1 1 4
    .word 9 8 1 7 5 1 7 4 9 7 4 8 2 5 9 3 6 3 6 3 2 7 3 2
    .word 1 6 1 4 2 9 6 1 3 2 5 7 3 9 4 4 6 5 9 8 4 5 1 4
    .word 7 7 7 2 1 6 1 3 9 4 4 6 6 6 3 9 3 8 2 8 8 4 8 7
    .word 7 8 7 9 3 5 7 1 1 4 1 4 9 6 7 3 8 5 1 7 9 2 2 2
    .word 2 4 6 5 7 3 4 6 1 7 2 5 1 7 1 2 9 6 7 8 5 4 5 7
    .word 2 4 4 9 2 8 1 9 5 9 5 9 8 3 4 7 6 7 5 2 9 9 5 5
    .word 8 4 2 6 3 8 8 3 6 3 2 4 5 1 8 6 6 4 5 8 4 6 8 5
    .word 7 7 9 8 4 1 1 3 8 8 7 6 3 8 1 2 2 4 4 5 3 5 9 9
    .word 5 7 1 7 5 5 8 1 4 6 5 7 5 9 3 7 4 8 6 4 1 6 7 1
    .word 4 5 3 3 1 2 5 3 1 5 7 6 6 2 8 8 8 3 6 3 1 2 6 3
    .word 9 5 3 4 7 2 9 9 8 6 2 5 9 3 1 8 6 9 6 3 3 2 3 3
    .word 8 6 5 3 3 7 6 3 3 9 1 4 7 5 1 6 5 1 6 8 8 1 9 7
    .word 4 7 5 9 1 7 6 9 5 2 3 7 3 8 8 3 9 8 5 6 1 6 6 9
    .word 2 8 6 9 3 3 6 9 4 5 2 6 3 8 3 9 6 7 6 5 6 8 2 6
    .word 4 8 6 4 5 3 9 4 3 4 7 9 9 4 5 8 6 6 3 4 7 1 3 4
    .word 7 4 6 7 1 9 6 2 8 4 5 6 7 6 4 1 6 3 1 2 5 9 2 1
    .word 2 8 9 1 6 5 1 7 2 3 3 5 4 8 6 1 9 8 5 8 1 4 4 7
    .word 8 8 2 9 9 4 8 8 9 2 6 4 2 8 1 2 3 3 9 5 3 1 1 1
    .word 3 9 5 7 7 9 7 3 4 2 1 8 6 3 6 9 3 3 4 2 5 1 2 3
    .word 4 4 6 4 5 8 1 7 4 4 6 6 9 7 9 4 3 6 6 4 9 8 2 6
    .word 3 8 2 2 7 4 3 8 7 4 1 6 6 2 3 5 2 1 8 4 6 4 8 6
    .word 5 2 5 6 5 9 3 3 8 1 3 8 2 9 2 8 9 7 2 7 5 5 7 7
    .word 2 7 6 4 3 2 1 4 6 3 7 5 7 7 5 6 4 6 8 2 9 3 6 1
    .word 6 4 4 6 1 4 2 6 3 7 9 9 4 4 2 1 8 1 4 4 2 7 4 9
    .word 3 8 5 2 3 9 2 4 8 9 3 3 6 2 3 3 1 8 5 8 8 5 1 9
    .word 1 5 8 1 4 9 2 4 9 5 7 6 7 4 8 9 1 3 8 6 4 4 9 9
    .word 5 6 7 8 3 2 9 1 1 7 7 6 9 7 7 7 8 8 3 3 8 9 9 1
    .word 8 2 5 9 1 1 7 6 3 6 7 7 7 2 4 5 5 2 1 1 1 7 4 3
    .word 8 9 4 5 4 6 2 5 3 7 5 1 6 7 2 8 5 6 2 2 1 7 6 2
