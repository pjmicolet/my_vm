//This tokenizer was generated at time:2020-12-20 17:10:49.788590
#include "tokenizer.h"
#include <iostream>

std::set<std::string> operators = {"+", "-", "*", "/", "=", "=="};
std::set<char> separator = {';'};
std::set<char> curly_open = {'{'};
std::set<char> curly_close = {'}'};
std::set<char> paren_open = {'('};
std::set<char> paren_close = {')'};
std::set<char> array_open = {'['};
std::set<char> array_close = {']'};
std::set<std::string> keyword = {"while", "for", "if", "else", "do"};

auto getType(const std::string& token ) -> token_type {
    if(operators.find(token) != operators.end())
        return OPERATORS;
    if(token.size()==1 && separator.find(token.at(0)) != separator.end())
        return SEPARATOR;
    if(token.size()==1 && curly_open.find(token.at(0)) != curly_open.end())
        return CURLY_OPEN;
    if(token.size()==1 && curly_close.find(token.at(0)) != curly_close.end())
        return CURLY_CLOSE;
    if(token.size()==1 && paren_open.find(token.at(0)) != paren_open.end())
        return PAREN_OPEN;
    if(token.size()==1 && paren_close.find(token.at(0)) != paren_close.end())
        return PAREN_CLOSE;
    if(token.size()==1 && array_open.find(token.at(0)) != array_open.end())
        return ARRAY_OPEN;
    if(token.size()==1 && array_close.find(token.at(0)) != array_close.end())
        return ARRAY_CLOSE;
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
         case CURLY_OPEN:
             o << "curly_open";
            break;
         case CURLY_CLOSE:
             o << "curly_close";
            break;
         case PAREN_OPEN:
             o << "paren_open";
            break;
         case PAREN_CLOSE:
             o << "paren_close";
            break;
         case ARRAY_OPEN:
             o << "array_open";
            break;
         case ARRAY_CLOSE:
             o << "array_close";
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

