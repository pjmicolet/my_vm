//This tokenizer was generated at time:2020-12-20 17:10:49.788503
#pragma once
#include<vector>
#include<set>
#include<iostream>
#define TOKEN_SEP ' '
enum token_type { OPERATORS,SEPARATOR,CURLY_OPEN,CURLY_CLOSE,PAREN_OPEN,PAREN_CLOSE,ARRAY_OPEN,ARRAY_CLOSE,KEYWORD,LITERAL,IDENTIFIER };
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
