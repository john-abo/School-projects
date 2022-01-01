@echo off

javac "ca/ucalgary/cpsc331/Dictionary.java"
javac "ca/ucalgary/cpsc331/HashTable.java"
javac "ca/ucalgary/cpsc331/Tester.java"
java "ca/ucalgary/cpsc331/Tester"

del "ca\ucalgary\cpsc331\Dictionary.class"
del "ca\ucalgary\cpsc331\HashTable.class"
del "ca\ucalgary\cpsc331\Tester.class"