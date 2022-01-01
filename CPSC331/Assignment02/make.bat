@echo off

javac -d build -cp build "src/ca/ucalgary/cpsc331/Dictionary.java"
javac -d build -cp build "src/ca/ucalgary/cpsc331/RedBlackTree.java"
javac -d build -cp build "src/ca/ucalgary/cpsc331/Tester.java"
java -cp build "ca/ucalgary/cpsc331/Tester"
