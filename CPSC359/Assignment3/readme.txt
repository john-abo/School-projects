idiv assumes value at top of stack is less than or equal to the value
below it.

Example:

using the following commands
	BIPUSH	0x66
	BIPUSH	0x1b
	
	TOS:	1b
			66
	
	should return:
	
	TOS:	03
			15
	Where 3 is the quotient and 15 is the remainder