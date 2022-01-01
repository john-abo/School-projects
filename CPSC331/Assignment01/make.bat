@echo off

javac -d build -cp build "src/ca/ucalgary/cpsc331/PriorityQueue.java"
javac -d build -cp build "src/ca/ucalgary/cpsc331/MinHeap.java"
javac -d build -cp build "src/ca/ucalgary/cpsc331/Tester.java"
java -cp build "ca/ucalgary/cpsc331/Tester"
