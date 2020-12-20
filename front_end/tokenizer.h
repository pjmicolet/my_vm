//This tokenizer was generated at time:2020-12-20 16:55:52.164732
#pragma once
#include<vector>
#include<set>
#include<iostream>
#define TOKEN_SEP ' '
enum token_type { OPERATORS,SEPARATOR,ARRAY_SEPARATOR,KEYWORD,LITERAL,IDENTIFIER };
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
