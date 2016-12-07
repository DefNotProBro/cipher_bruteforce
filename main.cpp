/*
    Author(s): Terry Thibault, Nicolas Fry, Nicholas Barnes, Chesalon Taylor
*/

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
#include <locale>

#include <algorithm>

// some int corresponding to the amount of validArgs
const int maxArgs = 4;
const std::string WORDS_PATH = "words.txt";
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
shotgun/all -x

?? pick one from above ?? pick one or both from below ??

-c <ciphertextlocation>
-pl <plaintextcipherlocation>

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
      if(strcmp(argv[i], "-pl") == 0) {

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

  // all the args are good!
  return true;
}

std::string shift_cipher_decrypt(std::string ciphertext, int cshift) {

  // use a stringstream
  // std::stringstream ss;

  // // start at position where we start the shift
  int position = 0;

  // caesar cipher s
  while (ciphertext[position] != '\0') {
    if (ciphertext[position] >= 'a' && ciphertext[position]<='z') {

        // subtract A (which is 67) to give you the ranges of 0 to 26
        char shiftletter = ciphertext[position] - 'a';

        // move characters over with shift
        shiftletter -= cshift;

        // check to see if we rolled over, if so, adjust the number
        shiftletter =  shiftletter < 0 ? (26 + shiftletter) : shiftletter % 26;

        // add A (which is 67) to give you the original range
        ciphertext[position] = shiftletter + 'a';
    }

    // move to next character
    position++;
  }

  // return the new plaintext which is the original string
  return ciphertext;

}

std::string readInFromFile(std::string path) {

    std::stringstream ss;
    std::locale loc;
    std::string line;

    std::ifstream infile(path.c_str());
    if (!infile.is_open()) {
      std::cout << "Error opening file" << std::endl;
      return "";
    }

    while (std::getline(infile, line)) {

        // to uppercase
        for (std::string::size_type i=0; i<line.length(); ++i) {
          ss << std::tolower(line[i],loc);
        }

        ss << " ";
    }

    infile.close();

    // remove last new line character
    std::string toReturn = ss.str();
    toReturn.erase(toReturn.length()-1);

    return toReturn;
}

// Word recognition
bool is_word(std::string candidate) {
    std::string line;
    std::ifstream wordlist (WORDS_PATH);

    if (!wordlist.is_open()) {
        std::cout << "Error opening file" << std::endl;
        return false;
    }

    while (std::getline(wordlist, line)) {
        if (line.compare(candidate) == 0) {
            wordlist.close();
            return true;
        }
    }
    wordlist.close();
    return false;
}

bool is_english(std::string str) {

    float threshhold = 0.85;
    int hits = 0;

    std::string buf; // Buffer String
    std::stringstream ss(str); // Insert the string into a stream

    std::vector<std::string> tokens; // Vector to hold our tokens

    while(ss >> buf) {
        tokens.push_back(buf);
    }

    for (std::string s : tokens) {
        if (is_word(s)) { hits++;}
    }

    if (hits / tokens.size() >= threshhold) {
        return true;
    }

    else {
        return false;
    }
}

bool do_shift(std::string cipherText) {

  for(int i = 1; i < 26; i++) {
    std::string shift = shift_cipher_decrypt(cipherText, i);
    std:: cout<< shift << std::endl;
    if(is_english(shift)) {
      std::cout << "\nWe cracked it with Shift!:\n" << shift << std::endl;
      return true;
    }
  }
}

void string_permutation(std::string& orig, std::string& perm, std::vector<std::string>& controlVector)
{
	if (orig.empty())
	{
		controlVector.push_back(perm);
		return;
	}

	for (int i = 0; i < orig.size(); ++i)
	{
		std::string orig2 = orig;

		orig2.erase(i, 1);

		std::string perm2 = perm;

		perm2 += orig.at(i);

		string_permutation(orig2, perm2, controlVector);
	}
}

bool do_columnar(std::string cipherText) {
  //Working Variables
	int testKeylen;
	int columnSize;

	//This variable holds the temporary String Row calculations for each testKey
	std::vector <std::string> possibleRows;
	std::vector <std::string> controlVector;

	for (testKeylen = 2; testKeylen <= 6/*cipherText.size() / 2*/; testKeylen++) //Test keys 2 --> half the size of input
	{
		if (cipherText.size() % testKeylen != 0) //Only test keys which are factors of input text size. Prevents ICT
		{
			continue;
		}
		//cout << "rows: " << cipherText.size() / testKeylen << endl;
		columnSize = cipherText.size() / testKeylen;
		for (int i = 0; i < testKeylen; i++) //One iteration per column. Tokenize input into distinct columns
		{
			possibleRows.push_back(cipherText.substr((i*columnSize), columnSize));
		}

		std::string orig; //Form "ABCDE"
		std::string perm;

		//Initialize orig. Orig is the list of alphanumeric characters that will be permutated
		for (int j = 0; j < testKeylen; j++)
		{
			orig.push_back(((char)(65 + j)));
		}

		//Perform permutation for testKeyLen characters, store in controlVector
		string_permutation(orig, perm, controlVector);

		std::string solnString = "";
		int a;
		std::string ss;
		for (int x = 0; x < controlVector.size(); x++) //Iterate through all possible key permutations
		{
			solnString = "";
			//cout << "controlV[x]: " << controlVector[x] << endl;
			for (int z = 0; z < columnSize; z++) //Which character position are we looking at?
			{
				for (int y = 0; y < controlVector[x].size(); y++) //Iterate through each column. 0 --> numColumns. Aka testKeyLen
				{
					a = controlVector[x].at(y) - 65; //Which column are we looking at? Defined by controlVector permutation
					ss = possibleRows[a].substr(z, 1); //Substr from the desired column, at the desired character position
					solnString.append(ss);
				}
			}

      // replace underscores with spaces
      std::replace(solnString.begin(), solnString.end(), '_', ' ');
			std::cout << solnString << std::endl;
      if(is_english(solnString)) {
        std::cout << "\nWe cracked it with columnar transposition!:\n" << std::endl;
        return true;
      }
		}
		controlVector.clear();
		possibleRows.clear();
	}

  return false;
}

int main(int argc, char* argv[]) {

    // basic argument parsing
    if(argc == 1) {
      std::cout << "Not enough Arguments" << std::endl;
      return -1;
    }

    if(argc-1 > maxArgs) {
      std::cout << "Too many arguments" << std::endl;
      return -1;
    }

    if(!goodArgs(argc, argv)) {
      return -1;
    }

    if(shift) {

      if(do_shift(readInFromFile(cipherTextLocation))) {
        return 0;
      }

      return -1;
    }

    if(rowtr) {
      if(do_columnar(readInFromFile(cipherTextLocation))) {
        return 0;
      }

      return -1;
    }

    return 0;
}
