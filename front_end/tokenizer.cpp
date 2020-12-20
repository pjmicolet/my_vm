//This tokenizer was generated at time:2020-12-20 16:06:54.071337
#include "tokenizer.h"
#include <iostream>

std::set<std::string> operators = {"+", "-", "*", "/", "=", "=="};
std::set<char> separator = {'{', '(', ')', '}', ';'};
std::set<std::string> keyword = {"while", "for", "if", "else", "do"};

auto getType(const std::string& token ) -> token_type {
    if(operators.find(token) != operators.end())
        return OPERATORS;
    if(token.size()==1 && separator.find(token.at(0)) != separator.end())
        return SEPARATOR;
    if(keyword.find(token) != keyword.end())
        return KEYWORD;
    return IDENTIFIER;
}

auto operator << (std::ostream& o, const Token& token ) -> std::ostream& {
	o << "Value: " << token.value_ << " Type: ";
	switch(token.type_) {
		case IDENTIFIER:
			o << "Identfier";
			break;
		case KEYWORD:
			o << "Keyword";
			break;
		case SEPARATOR:
			o << "Separator";
			break;
		case OPERATORS:
			o << "Operator";
			break;
		case LITERAL:
			o << "Literal";
			break;
	}
	return o;
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

