# Sieve of Eratosthenes
# https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
main:

    # PUT YOUR CODE
    li $t0, 0           # i = 0
    li $t1, 1           # 1
loop1:
    bge $t0, 1000, end_loop1
    
    sb $t1, prime($t0)

    add $t0, $t0, 1
    b loop1
end_loop1:



    li $t0, 2           # i = 2
loop2:
    bge $t0, 1000, end_loop2
    
if:
    lb $t2, prime($t0)
    beqz $t2, end_if

	move $a0, $t0
	li $v0, 1
	syscall				# printf("%d", i);

    li $a0, '\n'        # printf("%c", '\n');
    li $v0, 11
    syscall

    mul $t3, $t0, 2     # int j = 2 * i;

    li $t1, 0
loop3:
    bge $t3, 1000, end_if
    
    sb $t1, prime($t3)

    add $t3, $t3, $t0
    b loop3

end_if:

    add $t0, $t0, 1
    b loop2
end_loop2:

    li $v0, 0           # return 0
    jr $31

.data
prime:
    .space 1000