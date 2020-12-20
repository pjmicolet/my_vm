#include "tokenizer.h"
#include <iostream>

std::set<char> separators = {';', '{', '(', ')', '}'};
std::set<char> operators = {'+','-','*','/','='};
std::set<std::string> keywords = {"if", "else", "while", "for"};

auto operator << (std::ostream& o, const Token& token ) -> std::ostream& {
	o << "Value: " << token.value_ << " Type: ";
	switch(token.type_) {
		case IDENTIFIER:
			o << "Identifier";
			break;
		case KEYWORD:
			o << "Keyword";
			break;
		case SEPARATOR:
			o << "Separator";
			break;
		case OPERATOR:
			o << "Operator";
			break;
		case LITERAL:
			o << "Literal";
			break;
	}
	return o;
}

auto getType(const std::string& token ) -> token_type {
	if(token.size()==1 && separators.find(token.at(0)) != separators.end())
		return SEPARATOR;
	if(token.size()==1 && operators.find(token.at(0)) != operators.end())
		return OPERATOR;
	if(token.find_first_not_of("0123456789") == std::string::npos)
		return LITERAL;
	if(keywords.find(token) != keywords.end())
		return KEYWORD;
	return IDENTIFIER;
}

Tokenizer::Tokenizer(const std::string& line) {
	std::string temp = "";
	for(auto& character : line) {
		if(separators.find(character) != separators.end() || character == TOKEN_SEP) {
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
