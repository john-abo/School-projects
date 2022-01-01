/* Gonna need to review my graph algorithms
   And figure out the data structures I need lols
   This is about to be bad
   Oh man, file IO time too
   This implementation is desctructive to the created graph but that should be ok since I don't intend to do anything else with the graph
   This is also going to be terrible inefficient because at this point I'm just implementing my own functions for the sake of getting the
   program to work lols.

   It appears that Prim doesn't necessarily give the shortest path between 2 nodes, but it was listed on the site as an algotithm that can
   be used. This did generate a minimum spanning tree so all the connections are the cheapest they can be between ALL nodes.
 */

// C++ libraries
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <list>
#include <algorithm>

// C libraries
#include <stdlib.h> // I honestly forgot what this was supposed to do lols

// Some nice macros
#define inFile "canadamap.txt"

// maybe I'll make a struct to represent the tree node, idk
typedef struct tnode
{
    std::string name;
    float dist;  // Sets default value of node to be inf
    tnode *pred; // Points to the predecessor of the tree node
    int searched = 0;
} tnode;

// This struct represents adjacent vectors
typedef struct ad
{
    std::string name;
    float weight;
} ad;

/*
    Adds nodes to adjacency list
*/
void printPath(std::vector<tnode> v, std::string src, std::string des) {
    tnode* end;
    std::string path = "";
    float dist = -1;
    int hops = -1;

    for (int i = 0; i < v.size(); i++) {
        if (v[i].name.compare(des) == 0) {
            dist = v[i].dist;
            end = &v[i];    // Turns out my only mistake was using the reference in a for each loop instead of the actual location
        }
    }

    while (end != nullptr && end != (end -> pred)) {
        path = " -> " + (end -> name) +  path;
        end = (end -> pred);
        hops++;
    }

    // Going to need to crop the first arrow because I can't format at all, I just want to get this done
    path = path.substr(4,path.size());

    // huh, so far this is kinda broken...
    // It appears that the pred of each node is broken, but the weight of the path is still calculated
    // as if it were goind down the determined path. how do I fix this? who knows lol
    std::cout << src << "\t" << des << "\t" << dist << "\t" << hops << "\t" << path << "\n";
}

// Checks if the node passed is adjacent to u
int isAdj (tnode u, tnode node, std::map<std::string, std::vector<ad>> aList) {

    for (ad n : aList[u.name]) {
        //std::cout <<  n.name << "," << node.name << "\n";
        if (node.name.compare(n.name) == 0) {
            //std::cout << node.name << " is adjacent to " << u.name << "\n";
            return 1;
        }
    }

    //std::cout << "Freeze\n";
    //while(1);

    return 0;
}

int allSearched(std::vector<tnode> n) {
    for (tnode x : n) {
        if (!x.searched) return 0;  // If one of them isn't searched yet, return false
    }
    return 1;
}

float w (tnode u, tnode n, std::map<std::string, std::vector<ad>> aList) {
    float ret = -1; // In theory, this is guaranteed to be replaced? Lets hope so lols

    for (ad x : aList[u.name]) {
        if (n.name.compare(x.name) == 0) ret = x.weight;
    }

    return ret;
}

tnode ex_min(std::vector<tnode> list) {
    // Just in case no node was found. Solely for debugging purposes
    tnode min = {"FAILED SEARCH",std::numeric_limits<float>::max(),nullptr};

    // Searches for new min
    for (tnode n : list) {

        //std::cout << "\n" << min.name << "," << min.dist << "\n" << n.name << "," << n.dist << "\n";
        if (n.dist < min.dist && !n.searched) {
            //std::cout << "Min found\n";
            min.name = n.name;
            min.dist = n.dist;
            min.pred = n.pred;
        }
    }

    //std::cout << "Target name: " << min.name << "\n";
    return min;
}

int main()
{
    // bruh moment
    std::map<std::string, std::vector<ad>> aList;   // Adjacency list to represent the graph
                                                    // Primarily used for 
    
    std::vector<tnode> vectors;                      // List of all vectors

    // Just going to need to read the list from the text file they gave

    /*
        File IO shenanigans
    */
    std::string rd;
    std::ifstream rdFile(inFile);

    // Data that'll be read from the file

    if (!rdFile.is_open())
    {
        std::cout << "Smth happened, oh no\n";
    }
    else
    {
        std::cout << "File opened successfully\n";
    }

    // Creates the graph as an adjacency list
    while (getline(rdFile, rd))
    {
        //std::cout << "Line " << rd << "\n";
        // Finds the first token, which will be the vector
        std::string node;
        std::size_t breakln = rd.find(" ");
        node = rd.substr(0, breakln);
        rd.erase(0, breakln + 1);

        // Find the second token, which is the adjacent vector
        std::string adj;
        breakln = rd.find(" ");
        adj = rd.substr(0, breakln);
        rd.erase(0, breakln + 1);

        // Using the rest of the string to assign the weight, should only be a number left
        float weight = atof(rd.c_str());

        // creates node using strings and weights given, adds to adjacency list
        ad entry = {adj, weight};
        aList[node].push_back(entry);
    }
    rdFile.close();

    // I'll also need to root the tree at YYC, then the printing should be
    // easy enough lols. this should work just fine since I'd just need to
    // get the predecessors of the node until null, which should be the
    // root node, or YYC

    /*
        Prim shenanigans
    */

    // Create a vector of all nodes
    // Relaxes that graph
    for (auto const &x : aList)
    {
        vectors.push_back(tnode());
        vectors.back().name = x.first;
        vectors.back().dist = std::numeric_limits<float>::max();
        vectors.back().pred = nullptr;
    }

    // Here goes nothing, time for actual algorithm shenanigans lols

    // LOL, I forgot to set the root's weight to 0
    // my bad
    // in this case, I'm rooting it in YYC
    for (int i = 0; i < vectors.size(); i++) {
        if (vectors[i].name.compare("YYC") == 0) {
            std::cout << "YYC found\n";
            vectors[i].dist = 0;
        }
        //std::cout << "Testing: " << vectors[i].name << ":" << vectors[i].dist <<  "\n";
    }

    // For testing, just to see which nodes were put in the vector
    //for (tnode n : vectors) std::cout << "Testing: " << n.name << ":" << n.dist <<  "\n";

    // Last 24 hours, lets go lads
    // Perhaps a bit late, but maybe an adjacency matrix would be better? Oh no...
    while (!allSearched(vectors)) {
        tnode u = ex_min(vectors);  // Extracts the Q with the lowest key I guess?
        tnode* s;

        // This loop is just meant to get the address of u, lets hope this works huh?
        for (int i = 0; i < vectors.size(); i++) {
            if (vectors[i].name.compare(u.name) == 0) s = &vectors[i];
        }

        for (int i = 0; i < vectors.size(); i++) {
            if (isAdj(u, vectors[i], aList)) {
                float nodeWeight = w(u, vectors[i], aList);
                if (!vectors[i].searched && vectors[i].dist > nodeWeight) {
                    vectors[i].pred = s;    // This right here, is causing me a big boo boo of a problem
                    vectors[i].dist = nodeWeight + u.dist;
                }
            }
        }

        // Marks node u as having been searched already
        for (int i = 0; i < vectors.size(); i++) {
            //std::cout << "removing\n";
            if (u.name.compare(vectors[i].name) == 0) {
                //std::cout << vectors[i].name << " was searched\n";
                vectors[i].searched = 1;
            }
        }

    }

    // Here I Guess I just need the code to print
    // Holy, I think I just did it
    // :bruh:
    for (tnode n : vectors) {
        std::cout << "Test: " << n.name << ":" << n.dist << "\n";
        if (n.pred != nullptr) std::cout << "\tPred: " << (n.pred) -> name << "\n";
    }

    std::cout << "Src\tDes\tDist\tHops\tPath\n";
    std::cout << "-------------------------------------\n";

    // CAN1: YUL
    printPath(vectors, "YYC", "YUL");
    
    // CAN2: YVR (It's technically burnaby, but they're also under vancouver)
    printPath(vectors, "YYC", "YVR");

    // CAN3: YHZ
    printPath(vectors, "YYC", "YHZ");

    // TEST. Just an extra test path to demonstrate that the cheapest tree was chosen in place of the shortest path
    // In this case, it was cheaper to make a path from YVR to YXS then YVR to YYC, than it would have to make a
    // path directly from YYC to YXS.
    printPath(vectors, "YYC", "YXS");
    return 0;
}