from datetime import datetime
import sys

base_class = "Tokenizer"
literal_token = "__decimal__"
identifier = "__any__"
separators = "__separator__"
token_sep_name = "TOKEN_SEP"
no_sets = [ literal_token, identifier ]

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
		if tokens[0] in no_sets:
			continue
		if tokens[0] == separators:
			tokens = tokens[1:]
		is_char = is_character( tokens )
		char_wrap = "'" if is_char else "\""
		token_set = "std::set<" + ( "char" if is_char else "std::string" ) + "> " + possible_token.lower() + " = {" + ", ".join( "{1}{0}{1}".format( token, char_wrap ) for token in tokens )+ "};\n"
		token_sets += token_set
	return token_sets

def get_identifier_typename( token_types: dict ) -> str:
	for token in token_types:
		if token_types[token][0] == identifier:
			return token

def get_separator_typename( token_types: dict ) -> str:
	for token in token_types:
		if token_types[token][0] == separators:
			return token

def generate_get_type( token_types: dict ) -> str:
	function_header = "auto getType(const std::string& token ) -> token_type {\n"
	ident = " "*4
	body = ""
	identifier_name = get_identifier_typename( token_types )
	separator_name = get_separator_typename( token_types )
	for possible_token in token_types:
		if possible_token == identifier_name:
			continue
		if possible_token == separator_name:
			continue
		tokens = token_types[ possible_token ]
		is_char = is_character( tokens )
		if tokens[0] == literal_token:
			line = """{0}if(token.find_first_not_of("0123456789") == std::string::npos)\n{0}{0}return {1};\n""".format(ident, possible_token.upper())
		elif is_char:
			line = "{0}if(token.size()==1 && {1}.find(token.at(0)) != {1}.end())\n{0}{0}return {2};\n".format(ident, possible_token.lower(), possible_token.upper())
		else:
			line = "{0}if({1}.find(token) != {1}.end())\n{0}{0}return {2};\n".format(ident, possible_token.lower(), possible_token.upper())
		body += line
	body += "{0}return {1};\n}}\n".format(ident, identifier_name)
	return function_header+body

def generate_type_cout_operator( token_types: dict ) -> str:
	switch_statement = """auto operator << (std::ostream& o, const Token& token ) -> std::ostream& {{\n{0}o << "Value: " << token.value_ << " Type: ";\n{0}switch(token.type_) {{\n""".format( " "*4 )
	for type_name in token_types:
		switch_statement += """{0} case {1}:\n{2} o << "{3}";\n{2}break;\n""".format( " "*8, type_name.upper(), " "*12, type_name.lower() )
	switch_statement += "{0}}}{0}return o;\n}}\n".format(" "*4 )
	return switch_statement;
	
def generate_tokenizer_init( separator_name : str ) -> str:
	function_header = "{0}::{0}(const std::string& line ) {{\n".format(base_class) 
	body="""	std::string temp = "";
	for(auto& character : line) {{
		if({0}.find(character) != {0}.end()) {{
			tokens_.emplace_back(Token(temp, getType(temp)));	
			temp = "";
			temp += character;
			tokens_.emplace_back(Token(temp, getType(temp)));
			temp = "";
		}}
		else if( character == TOKEN_SEP ) {{
			tokens_.emplace_back(Token(temp, getType(temp)));	
			temp = "";
		}}
		else {{			
			temp += character;
		}}
	}}
	if(temp.size() != 0)
		tokens_.emplace_back(Token(temp, getType(temp)));\n}}""".format( separator_name )
	return function_header + body

def generate_tokenizer_getter() -> str:
	return """const std::vector<Token>& Tokenizer::getTokens() const {
	return tokens_;\n}\n"""

def generate_enum( token_types : dict ) -> str:
	return "enum token_type {{ {0} }};".format( ",".join( t_type for t_type in token_types ) )

def generate_token_struct() -> str:
	return """struct Token {
	Token( std::string value, token_type type ) : value_(value), type_(type) {};
	const std::string value_;
	const token_type type_;
};\nauto operator << (std::ostream& o, const Token& token ) -> std::ostream&;\n"""

def generate_token_class_header() -> str:
	return """class Tokenizer {

public:
	Tokenizer(const std::string& path);
	const std::vector<Token>& getTokens() const;

private:
	std::vector<Token> tokens_;
};"""


def generate_tokenizer( path : str, cpp_file: str, h_file: str ) -> None:
	token_types = {}
	cpp_includes = """#include "{0}"\n#include <iostream>\n""".format(h_file)
	h_includes = """#pragma once\n#include<vector>\n#include<set>\n#include<iostream>""".format(h_file)
	with open( path ) as f:
		line = f.readline()
		while line:
			token_name, token_list = line.split(":")[0], line.split(":")[1]
			token_types[ token_name ] =  generate_list( token_list )
			line = f.readline()

	with open( h_file, "w" ) as w_h_file:
		w_h_file.write( generate_timestamp() + "\n" )
		w_h_file.write( h_includes +"\n" )
		w_h_file.write( "#define {0} ' '\n".format( token_sep_name ) )
		w_h_file.write( generate_enum(token_types) + "\n" )
		w_h_file.write( generate_token_struct() + "\n" )
		w_h_file.write( generate_token_class_header() + "\n" )
	with open( cpp_file, "w" ) as w_cpp_file:
		w_cpp_file.write( generate_timestamp() +"\n" )
		w_cpp_file.write( cpp_includes +"\n" )
		w_cpp_file.write( generate_sets( token_types ) + "\n" )
		w_cpp_file.write( generate_get_type( token_types ) +"\n")
		w_cpp_file.write( generate_type_cout_operator( token_types ) +"\n" )
		w_cpp_file.write( generate_tokenizer_init( get_separator_typename( token_types ).lower() ) +"\n" )
		w_cpp_file.write( generate_tokenizer_getter() + "\n")

generate_tokenizer( sys.argv[1], sys.argv[2], sys.argv[3] )
