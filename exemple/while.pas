	.data
a: 	.word
space: .word ' '
	.text

main:

	#read
	addi $v0, $zero, 5
	syscall
	add $t0, $v0, $zero
	sw $t0, a

	lw $t0, a
	addi $t1, $zero, 2
	add $t0, $t1, $t0

	#write
	add $a0, $t0, $zero
	addi $v0, $zero, 1
	syscall
	#space
	lw $a0, space
	addi $v0, $zero, 11
	syscall

	#exit
	addi $v0, $zero, 10
	syscall
