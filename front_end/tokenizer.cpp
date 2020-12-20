//This tokenizer was generated at time:2020-12-20 16:49:28.892742
#include "tokenizer.h"
#include <iostream>

std::set<std::string> operators = {"+", "-", "*", "/", "=", "=="};
std::set<char> separator = {'{', '(', ')', '}', ';'};
std::set<char> array_separator = {'[', ']'};
std::set<std::string> keyword = {"while", "for", "if", "else", "do"};
std::set<std::string> literal = {"__decimal__"};
std::set<std::string> identifier = {"__any__"};

auto getType(const std::string& token ) -> token_type {
    if(operators.find(token) != operators.end())
        return OPERATORS;
    if(token.size()==1 && separator.find(token.at(0)) != separator.end())
        return SEPARATOR;
    if(token.size()==1 && array_separator.find(token.at(0)) != array_separator.end())
        return ARRAY_SEPARATOR;
    if(keyword.find(token) != keyword.end())
        return KEYWORD;
    if(token.find_first_not_of("0123456789") == std::string::npos)
        return LITERAL;
    return IDENTIFIER;
}

auto operator << (std::ostream& o, const Token& token ) -> std::ostream& {
    o << "Value: " << token.value_ << " Type: ";
    switch(token.type_) {
         case OPERATORS:
             o << "operators";
            break;
         case SEPARATOR:
             o << "separator";
            break;
         case ARRAY_SEPARATOR:
             o << "array_separator";
            break;
         case KEYWORD:
             o << "keyword";
            break;
         case LITERAL:
             o << "literal";
            break;
         case IDENTIFIER:
             o << "identifier";
            break;
    }    return o;
}

Tokenizer::Tokenizer(const std::string& line ) {
	std::string temp = "";
	for(auto& character : line) {
		if(separator.find(character) != separator.end() || character == TOKEN_SEP) {
			tokens_.emplace_back(Token(temp, getType(temp)));	
			temp = "";
		}
		else {			
			temp += character;
		}	
	}
	if(temp.size() != 0)
		tokens_.emplace_back(Token(temp, getType(temp)));
}
const std::vector<Token>& Tokenizer::getTokens() const {
	return tokens_;
}

