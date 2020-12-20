#pragma once
#include<vector>
#include<string>
#include<set>
#include<iostream>

#define TOKEN_SEP ' '

enum token_type { IDENTIFIER, KEYWORD, SEPARATOR, OPERATORS, LITERAL }; 

struct Token {
	Token( std::string value, token_type type ) : value_(value), type_(type) {};
	const std::string value_;
	const token_type type_;
};

auto operator << (std::ostream& o, const Token& token ) -> std::ostream&;

class Tokenizer {

public:
	Tokenizer(const std::string& path);
	const std::vector<Token>& getTokens() const;

private:
	std::vector<Token> tokens_;
};
