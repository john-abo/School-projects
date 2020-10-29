outfmt:	.string "Random number %d: %d its double: %d \n"   
//printf("Random number %d: %d its double: %d \n", i, r_i, 2*ri)
outfmt2: .string "min is %d, max is %d, sum is %d \n"





		.balign 4
		.global main

main:	
		stp 	x29, 	x30, 	[sp, -16]!
		mov		x29, 	sp

		mov 	x19,	1

		//save    min in x21, max in x22, sum in x14
		mov		x21,	10
		mov		x22, 	0
		mov		x23,	0
		
		// seed rand()
		mov 	x0, 	0
		bl 		time
		bl 		srand
		b pretest

loop: 
		
		bl 		rand  // Returned value will be saved in x0
		//ldr 	x10, 	=0x09
		
		// Returned value is saved in x0, and 0xFF limits it between 0-255, save the new value in x2 .  0 - 10    and  x2, x0, 0x0A 0000...00011111111
		// this 4 lines of instrunctions implements: %9, reminder saved in x2, x11 = x0/x10,  x2 = x0 - x11*x10
		mov 	x10, 	9
		sdiv  	x11, 	x0,  	x10			
		mul		x2, 	x11, 	x10
		sub     x2, 	x0,  	x2
		add 	x2, 	x2, 	1

		// save the sum in x14
sum:	add		x23, 	x23, 	x2
 
		// 	save the min in x12, max in x13
cmp1:	cmp 	x2, 	x21
		b.gt  	cmp2
		mov 	x21, 	x2
		
cmp2: 	cmp 	x2, 	x22
		b.lt 	endcmp
		mov 	x22, 	x2

endcmp:	ldr 	x0, 	=outfmt
		mov	 	x1, 	x19
		add		x3, 	x2, 	x2  		// Save the double in x3
		bl 		printf
		add 	x19, 	x19, 	1


pretest:
		cmp 	x19,	10
		b.le 	loop

		// print the min, max and sum.
		ldr 	x0, 	=outfmt2
		mov 	x1, 	x21
		mov		x2, 	x22
		mov		x3, 	x23
		bl 		printf

		//ret
		ldp 	x29, 	x30, 	[sp], 	16
		ret

//Such an immediate is a 32-bit or 64-bit pattern viewed as a vector of identical elements of size e = 2, 4, 8, 16, 32, or 64 bits. 
//Each element contains the same sub-pattern: a single run of 1 to e-1 non-zero bits, rotated by 0 to e-1 bits. 
//This mechanism can generate 5,334 unique 64-bit patterns (as 2,667 pairs of pattern and their bitwise inverse). 
//Because the all-zeros and all-ones values cannot be described in this way, the assembler generates an error message.
