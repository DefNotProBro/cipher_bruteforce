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

#include <algorithm>

// some int corresponding to the amount of validArgs
const int maxArgs = 6;
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

//Method decrypts a rail cipher based on a single depth
std::string railDec(std::string cipher, int depth){

	int z = (2*depth)-2; int x = 0; std::string text[depth];
	int segment = cipher.length()/z; int extra = cipher.length()%z;
	std::string plain = "";

	//This for loop is used to initialize the rows, dividing the cipher text into the individual rows
	for(int i = 0; i < depth; i++){
		//this y variable will be used to represent the length of a given row i
		int y = 0;
		//For initializing the first and last rows of the rail cipher
		//These two rows have only one entry per completed segment
		//extra if statements add onto the size
		if(i == 0 || i == depth-1){
			y = segment;
			if(extra > 0 && i == 0){y += 1;}
			if(extra >= depth && i == depth-1){y += 1;}
		}
		//This else initializes the intermediate rows of the rail cipher
		//These rows will have two entries per completed segment
		//there are also if statements for adding more onto y if needed
		else{
			y = 2 * segment;
			if(extra > z-i){y += 2;}
			else if(extra > i){y += 1;}
		}
		//Finally, this sets the row value to the text array
		//y is added to x immediately after so that the next row won't pull values that already belong to the one before it
		text[i] = cipher.substr(x,y); x += y;
	}
	//This for loop then reconstructs the original message, pulling a variable from each row in the required order
	for(int j = 0; j < cipher.length(); j++){
		int a = j%z;
		//this if statement handles when you're moving up the rail and reading the rows in reverse order
		if(a >= depth){a = z - a;}
		plain += text[a].at(0);
		text[a] = text[a].substr(1,text[a].length()-1);
	}
	//returns the plaintext
	return plain;
}

bool do_viginere(std::string plain, std::string cipher){

	//Takes in both cipher and plain text and initializes a key
	std::string key = ""; std::string plains = "";
	//if(plain.length() != cipher.length()){return "ERROR: Mismatching string lengths!";}
	for(int i = 0; i < plain.length(); i++){
		if(plain.at(i) == ' '){continue;}
		if(plain.at(i) < 97){plain.at(i) += 32;}
		if(cipher.at(i) < 97){cipher.at(i) += 32;}
		int c = cipher.at(i) - plain.at(i); if(c < 0){c += 26;} c += 97;
		key += (char) c; plains += plain.at(i);
	}

	//Second for loop analyzes the full key to find the specific key word whether its periodic or autokeyed
	for(int j = 1; j < key.length(); j++){

		//Handles a periodic viginere key
		if(key.at(j) == key.at(0)){
			int l = 0; bool loop = false; int k;
			for(k = 1; k < key.length()/(j-1); k++){
				if(key.substr(l*j,j) == key.substr(k*j,j)){loop = true; l++;}
				else{loop = false; break;}
			}

			//in event that a periodic key doesn't fully loop this ensures that it doesn't go by unnoticed
			if(key.length()%(j-1) > 0){
				if(key.substr(k*j, key.length()-k*j) == key.substr(0, key.length()-(k*j))){loop = true;}
				else{loop = false;}
			}
			if(loop == true){
      				std::cout << "\nWe found a periodical Viginere Key!:\n" << key.substr(0,j) << std::endl;
      				return true;
			}
		}

		//Handles an auto-keyed viginere key
		if(key.at(j) == plain.at(0)){
			if(key.substr(j, key.length()-j) == plains.substr(0, key.length()-j)){
				std::cout << "\nWe found an autokeyed Viginere Key!:\n" << key.substr(0,j) << std::endl;
      				return true;
			}
		}
	}
	//this key will only be returned if it is neither periodic or auto-keyed and that the full key length is uniquely defined
	std::cout << "\nWe found a Viginere Key!:\n" << key << std::endl;
      	return true;
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

  return false;
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

//This method checks all possible depths by going through a for loop for every possible depth
bool do_rail(std::string cipher){
	for(int i = 2; i < cipher.length(); i++){
		std::string rail = railDec(cipher,i);
		std:: cout<< rail << std::endl;
		if(is_english(rail)){
      			std::cout << "\nWe cracked it with a Rail cipher of depth " << i << "!:\n" << rail << std::endl;
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

    if(rail) {

      if(do_rail(readInFromFile(cipherTextLocation))) {
        return 0;
      }

      return -1;
    }

   if(vige) {

      if(do_viginere(readInFromFile(plainTextLocation), readInFromFile(cipherTextLocation))) {
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

    if(shotgun) {

      if(!plainTextLocation.empty()) {
        if(do_viginere(readInFromFile(plainTextLocation), readInFromFile(cipherTextLocation))) {
          return 0;
        }

        return -1;
      }

      if(do_shift(readInFromFile(cipherTextLocation))) {
        return 0;
      }

      if(do_rail(readInFromFile(cipherTextLocation))) {
        return 0;
      }

      if(do_columnar(readInFromFile(cipherTextLocation))) {
        return 0;
      }

      mono_sub(readInFromFile(cipherTextLocation));
     	return 0;
    }

    return 0;
}
