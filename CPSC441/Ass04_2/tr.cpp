// C++ libraries
#include <iostream>
#include <fstream>
#include <vector>

// C libraries
#include <math.h>
#include <stdlib.h>

// Defining the file that will be treated as the customer basket
#define inFile 

int coll = 0;
int succ = 0;
int slot = 0;
int idle = 0;

// Number of collisions, Successes, and time Slots
int K = 10;     // Imma set this here real quck before I start
int M = (int) pow(2, K);    // This isn't really necessary tbh, but that's ok

// Doing this recursively made it so much easier to implement and debug
// scan represents the list of items that exist with the code up to that point
void treeScan(std::vector<int> scan, int depth) {
    std::vector<int> ones;
    std::vector<int> zero;

    // checks the list of items that will be scanned
    for (int a : scan) {
        // Time for some bit mask shenanigans LOL
        int id = ((a >> (K - depth)) & (0b0000000000000001));   // I love this binary number, because I 100% should've just said "& 1"
        
        if (id) {
            ones.push_back(a);
        } else {
            zero.push_back(a);
        }
    }
    
    // check for collisions
    if (ones.size() + zero.size() > 1) {

        if (ones.size() > 1 && zero.size() == 0) {
            idle++;
        }

        if (ones.size() == 0 && zero.size() > 1) {
            idle++;
        }

        coll++;
        treeScan(ones,(depth + 1));
        treeScan(zero,(depth + 1));
    }
    slot++;
}

// This should be fine right?
int run(int inInt) {

    // Creates list of items and populates them
    std::vector<int> scan;

    // Actual work
    coll = 0; succ = 0; slot = 0; idle = 0;

    // I think I'll start by reading the file of items
    // Just going to assume perfect inputs everytime, can't be bothered to check for mistakes this time
    std::string rd;
    std::string dir = "testsK10/customer"; dir += inInt + 48; dir += ".txt";
    std::cout << "File: " << dir << "\n";
    std::ifstream rdFile(dir.c_str());
    if (!rdFile.is_open()) std::cout << "Smth happened, oh no...\n";
    while (getline(rdFile, rd)) scan.push_back(atoi(rd.c_str()));
    // Another debugging line, just to see if the file was read correctly // for (int a : scan) std::cout << "Bruh: " << a << "\n";

    // To answer one question, I'll just print the size of the scanning list
    std::cout << "Number of items: " << scan.size() << "\n";

    treeScan (scan, 1);
    std::cout << "Root probe (" << ((double)scan.size() * 100/slot) << "%):\n\tCollisions: " << coll << "\n\tSuccesses: " << scan.size() << "\n\tSlots: " << slot << "\n\tIdle: " << idle << "\n";

    // Probing at the leaves, the efficiency can be calculated. number of successes is number of items being scanned,
    // and number of slots is number of possible items that can be scanned. All with no collisions

    // These values can be hard coded for
    //treeScan (scan,K);    // This used to work???
    std::cout << "Leaf probe (" << ((double)scan.size() * 100/M) << "%):\n\tCollisions: " << 0 << "\n\tSuccesses: " << scan.size() << "\n\tSlots: " << M << "\n\tIdle: " << (M - scan.size()) << "\n";
    
    // Lets get this bread
    rdFile.close();
    return 0;
}
// rewrote it so it just ran through all the files manually
int main() {for (int i = 1; i < 10; i++) run(i); std::cout << "It is done\n"; return 0;}