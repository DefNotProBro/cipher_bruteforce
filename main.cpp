/*
    Author(s): Terry Thibault, Nicolas Fry, Nicholas Barnes, Chesalon Taylor
*/

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>

// some int corresponding to the amount of validArgs
const int maxArgs = 3;

// different algorithms to make
bool shift = false;
bool mono = false;
bool play = false;
bool hill = false;
bool vige = false;
bool rail = false;
bool rowtr = false;
bool shotgun = false;

// Locations of input files
std::string cipherTextLocation;
std::string plainTextLocation;

/*
shift -s
mono -m
play -p
hill -h
vige -v
Rail -r
RowTr -t
shotgun -x

?? pick one from above ?? pick one or both from below ??

-c <ciphertextlocation>
-p <plaintextcipherlocation>
*/

// configures the program to check and make sure that our arguments are good
bool goodArgs(const int argc, char* argv[]) {

  // counters to make sure we do not go over 1 algoArg

  int algoArgs = 0;
  // check against different args and
  for(int i = 1; i < argc; i++) {

      // set shotgun to true
      if(strcmp(argv[i], "-x") == 0) {
        shotgun = true;
        algoArgs++;
      }

      // set shift to true
      if(strcmp(argv[i], "-s") == 0) {
        shift = true;
        algoArgs++;
      }

      // set play to true
      if(strcmp(argv[i], "-p") == 0) {
        play = true;
        algoArgs++;
      }

      // set hill to true
      if(strcmp(argv[i], "-h") == 0) {
        hill = true;
        algoArgs++;
      }

      // set vige to true
      if(strcmp(argv[i], "-v") == 0) {
        vige = true;
        algoArgs++;
      }

      // set rail to true
      if(strcmp(argv[i], "-r") == 0) {
        rail = true;
        algoArgs++;
      }

      // set rowtr to true
      if(strcmp(argv[i], "-t") == 0) {
        rowtr = true;
        algoArgs++;
      }

      // if we have a ciphertext path
      if(strcmp(argv[i], "-c") == 0) {

        // check to see we dont go over the boundry
        if(i+1 < argc) {
          cipherTextLocation = argv[i+1];
        } else {
          std::cout << "Did not provide a ciphertext argument" << std::endl;
          return false;
        }
      }

      // if we have a ciphertext path
      if(strcmp(argv[i], "-p") == 0) {

        // check to see we dont go over the boundry
        if(i+1 < argc) {
          plainTextLocation = argv[i+1];
        } else {
          std::cout << "Did not provide a plaintext argument" << std::endl;
          return false;
        }
      }

  }

  if(algoArgs > 1) {
    std::cout << "You may only have one algorithm argument" << std::endl;
    return false;
  }

  if(algoArgs == 0) {
    std::cout << "Check your args!" << std::endl;
    return false;
  }

  return true;
}

std::string readInFromFile(std::string path) {

    std::stringstream ss;
    std::string line;

    std::ifstream infile(path.c_str());
    if (!infile.is_open()) {
      std::cout << "Error opening file" << std::endl;
      return "";
    }

    while (std::getline(infile, line)) {
        ss << line;
        ss << "\n";
    }

    infile.close();

    return ss.str();
}

int main(int argc, char* argv[]) {

    // basic argument parsing
    if(argc == 1) {
      std::cout << "Not enough Arguments" << std::endl;
      return -1;
    }

    if(argc-1 > maxArgs) {
      std::cout << "To many arguments" << std::endl;
      return -1;
    }

    if(!goodArgs(argc, argv)) {
      return -1;
    }

    // testing file reading. Needs to be absolute path
    std:: cout<< readInFromFile(cipherTextLocation) << std::endl;

    return 0;
}
