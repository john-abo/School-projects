V 1.0

To compile, simply run g++ tr.cpp. The value of K is set as the variable K
in the code, and should be changed before compiling. Another thing to set
before compiling is the file that will be read (Assumed to be a perfect
input, and is not checked). This is done with the #define inFile <file name>
where <file name> is the location of the text file relative to the executable.

Reads from a file of integers (As given by the professor)

My file structure while testing placed all the test files into 2 folders named:
    testsK3 (Files for K = 3)
    testK10 (Files for K = 10)
and the file name was changed before compiling.

Note the value of K. This is very important as failing to set the currect value
cause the bit shifting to fail.
