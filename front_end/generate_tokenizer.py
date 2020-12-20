from datetime import datetime
import sys

includes = """#include "tokenizer.h"\n#include <iostream>\n"""
base_class = "Tokenizer"

def generate_timestamp() -> str:
	return "//This tokenizer was generated at time:{0}".format( datetime.now() )

def generate_list( data : str ) -> str:
	return [ token.strip() for token in data.split(",") ]

def is_character( data : list ) -> bool:
	return len( max( data, key=len) ) == 1

def generate_sets( token_types : dict ) -> str:
	token_sets = ""
	for possible_token in token_types:
		tokens = token_types[ possible_token ]
		is_char = is_character( tokens )
		char_wrap = "'" if is_char else "\""
		token_set = "std::set<" + ( "char" if is_char else "std::string" ) + "> " + possible_token.lower() + " = {" + ", ".join( "{1}{0}{1}".format( token, char_wrap ) for token in tokens )+ "};\n"
		token_sets += token_set
	return token_sets

def generate_get_type( token_types: dict ) -> str:
	function_header = "auto getType(const std::string& token ) -> token_type {\n"
	ident = " "*4
	body = ""
	for possible_token in token_types:
		tokens = token_types[ possible_token ]
		is_char = is_character( tokens )
		if is_char:
			line = "{0}if(token.size()==1 && {1}.find(token.at(0)) != {1}.end())\n{0}{0}return {2};\n".format(ident, possible_token.lower(), possible_token.upper())
		else:
			line = "{0}if({1}.find(token) != {1}.end())\n{0}{0}return {2};\n".format(ident, possible_token.lower(), possible_token.upper())
		body += line
	body += "{0}return IDENTIFIER;\n}}\n".format(ident)
	return function_header+body

def generate_type_cout_operator() -> str:
	return """auto operator << (std::ostream& o, const Token& token ) -> std::ostream& {
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
}\n"""
	
def generate_tokenizer_init() -> str:
	function_header = "{0}::{0}(const std::string& line ) {{\n".format(base_class) 
	body="""	std::string temp = "";
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
		tokens_.emplace_back(Token(temp, getType(temp)));\n}"""
	return function_header + body

def generate_tokenizer_getter() -> str:
	return """const std::vector<Token>& Tokenizer::getTokens() const {
	return tokens_;\n}\n"""

def generate_tokenizer( path : str, filename: str ) -> None:
	token_types = {}
	with open( path ) as f:
		line = f.readline()
		while line:
			token_types[ line.split(":")[0] ] =  generate_list( line.split(":")[1] )
			line = f.readline()

	with open( filename, "w" ) as cpp_file:
		cpp_file.write( generate_timestamp() +"\n" )
		cpp_file.write( includes +"\n" )
		cpp_file.write( generate_sets( token_types ) + "\n" )
		cpp_file.write( generate_get_type( token_types ) +"\n")
		cpp_file.write( generate_type_cout_operator() +"\n" )
		cpp_file.write( generate_tokenizer_init() +"\n" )
		cpp_file.write( generate_tokenizer_getter() + "\n")

generate_tokenizer( sys.argv[1], sys.argv[2] )
