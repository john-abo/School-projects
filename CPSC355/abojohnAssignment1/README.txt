Assignment 1 README

The program either produces a random table based on the sizes given in the
command line, or from a table if the user provides the file name to a file
with an existing table. The side length of the table must be between 5 to 20,
and the given size must reflect this. When a file name is given, the sizes
provided in the command line must match the dimensions of the table given.

The program is designed to be similar to search engine where the rows of the
table represent documents, and the columns represent a word in the document.
Each element in the table represent the frequency of word [j] in document [i].

The user will then be prompted for the index of word they want the search
to be based on and the number of documents they want returned. They will then
recieve the index of the top documents based on the frequency of the words.

As the program runs the table used, user input, and the result of the search
are logged to to assign1.log.

For the files with existing tables:
	occurences1.txt is 8x8
	occurences2.txt is 5x5

The sorting algorithm used is a selection sort implementation I recycled
from CPSC 233, rewritten for C.

The zip file contains the source code and code that has been compiled, as well
as two text files that contain existing tables. There is also the .log file
along with the script file.