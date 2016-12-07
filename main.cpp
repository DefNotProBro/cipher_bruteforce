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
#include <unordered_map>
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

      // set mono to true
      if(strcmp(argv[i], "-m") == 0) {
        mono = true;
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

// Mono alphabetic substitution
std::string mono_sub(std::string ciphertext) {
	std::string plaintext = "";
	int size = ciphertext.length();
	
	// Add all of the frequencies from the English language, according to
	// Cornel University. https://www.math.cornell.edu/~mec/2003-2004/cryptography/subs/frequencies.html
	char freq[26];
	freq[0]  = 'e'; 
	freq[1]  = 't';
	freq[2]  = 'a';
	freq[3]  = 'o';
	freq[4]  = 'i';
	freq[5]  = 'n';
	freq[6]  = 's';
	freq[7]  = 'r';
	freq[8]  = 'h';
	freq[9]  = 'd';
	freq[10] = 'l';
	freq[11] = 'u';
	freq[12] = 'c';
	freq[13] = 'm';
	freq[14] = 'f';
	freq[15] = 'y';
	freq[16] = 'w';
	freq[17] = 'g';
	freq[18] = 'p';
	freq[19] = 'b';
	freq[20] = 'v';
	freq[21] = 'k';
	freq[22] = 'x';
	freq[23] = 'q';
	freq[24] = 'j';
	freq[25] = 'z';
	
	// Initialize the counter variables
	std::unordered_map<char, int> count;
	count['a'] = 0;
	count['b'] = 0;
	count['c'] = 0;
	count['d'] = 0;
	count['e'] = 0;
	count['f'] = 0;
	count['g'] = 0;
	count['h'] = 0;
	count['i'] = 0;
	count['j'] = 0;
	count['k'] = 0;
	count['l'] = 0;
	count['m'] = 0;
	count['n'] = 0;
	count['o'] = 0;
	count['p'] = 0;
	count['q'] = 0;
	count['r'] = 0;
	count['s'] = 0;
	count['t'] = 0;
	count['u'] = 0;
	count['v'] = 0;
	count['w'] = 0;
	count['x'] = 0;
	count['y'] = 0;
	count['z'] = 0;

	for(int i = 0; i < ciphertext.length(); ++i) {
		if (ciphertext.at(i) != ' ') {
			count[ciphertext.at(i)]++;
		}
	}
	
	for(int i = 0; i < 26; ++i) {
		std::cout << freq[i] << " :: " << count[freq[i]] << std::endl;
	}
	
	std::unordered_map<char, char> assoc;
	int max, max_index;
	int fr = 0;
	for(int i = 0; i < 26; ++i) {
		max = 0;
		for(int j = 0; j < 26; ++j) {
			if(count[freq[j]] > max) {
				max = count[freq[j]];
				max_index = j;
			}
		}
		
		assoc[freq[i]] = freq[max_index];
		
	count[freq[max_index]] = 0;
	}
	
	for(int i = 0; i < 26; ++i) {
		std::cout << freq[i] << " :: " << assoc[freq[i]] << std::endl;
	}
	
//	std::replace(ciphertext.begin(), ciphertext.end(), 'k', char('e' - 32));	
	for(int i = 0; i < 26; ++i) {
		std::cout << ciphertext << std::endl << std::endl;
		std::cout << freq[i] << "," << assoc[freq[i]] << std::endl << std::endl;
		std::replace(ciphertext.begin(), ciphertext.end(), assoc[freq[i]], char(freq[i] - 32));	
	} 
	
	std::cout << ciphertext;
	
	return "";
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
    
    if(mono) {
    	mono_sub(readInFromFile(cipherTextLocation));
     	return 0;
    }
    
    if(shift) {
    	

      if(do_shift(readInFromFile(cipherTextLocation))) {
        return 0;
      }

      return -1;
    }

    return 0;
}
