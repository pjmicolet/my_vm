#include "tokenizer.h"
#include <fstream>
#include <iostream>

int main() {
	
	std::ifstream file("test.txt");
	std::string line = "";
	if(file.is_open()) {
		while(std::getline(file,line)) {
			Tokenizer tokens(line);
			for(auto& token : tokens.getTokens())
				std::cout<< token << " || ";
			std::cout << "\n";
		}
	}	
}
