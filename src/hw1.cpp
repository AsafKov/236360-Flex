#include <iostream>
#include <stdio.h>
#include <string>
#include "tokens.hpp"

void parse_token(int token);
void handle_string();
void handle_hex_escape();
void handle_error(int errorType, char* error_seq=NULL);
void print_token(std::string tokenName);
void handle_string();
void print_string();
void handle_escape_sequence();

std::string TOKEN_NAMES[] = {"VOID", "INT", "BYTE", "B","BOOL", "AND", "OR", "NOT", "TRUE", "FALSE",
    "RETURN", "IF", "ELSE", "WHILE", "BREAK", "CONTINUE",  "SC", "COMMA", "LPAREN", "RPAREN",
    "LBRACE", "RBRACE", "ASSIGN", "RELOP", "BINOP", "COMMENT", "ID", "NUM", "STRING","OVERRIDE", 
    "UNCLOSED_STRING"};
char string_buffer[1025];
int buffer_position = 0;
const int VALID_RANGE_START = 0;
const int VALID_RANGE_END = 127;
const std::string HEX_CHARS = "0123456789abcdefABCDEF"; 
const std::string ESCAPE_CHARS = "0nrt\\\""; 
enum ErrorType{
    UNDEFINED_ESCAPE_ERROR = 1,
    INVALID_CHAR_ERROR = 2,
    UNCLOSED_STRING_ERROR = 3
};

int main(){
    int token;
	while ((token = yylex())) {
	    parse_token(token);
    }
    if(buffer_position != 0){
      handle_error(UNCLOSED_STRING_ERROR);
    }
	return 0;
}

void parse_token(int token){
    switch(token){
      case STRING: {
        handle_string();
      } break;
      case COMMENT: {
        printf("%d %s %s\n", yylineno, "COMMENT", "//");
      } break;
      default: {
        for(int i= VOID; i<=OVERRIDE; i++){
            if(token == i){
                print_token(TOKEN_NAMES[i-1]);
            }
        }
      }           
    }
}

void handle_string(){
    if(yytext[0] == '\"'){
      print_string();
      return; 
    } else {
      if(buffer_position == 1024){
        handle_error(UNCLOSED_STRING_ERROR);
      }
    }

    if(yytext[0] == '\\'){
        handle_escape_sequence(); 
     } else {
        for(int i=0; i<yyleng; i++){
            if(yytext[i] == '\n' || yytext[i] == '\r'){
                handle_error(UNCLOSED_STRING_ERROR);
            }
            string_buffer[buffer_position+i] = yytext[i];
            buffer_position++;
        }
     }
}


void print_string(){
    string_buffer[buffer_position] = '\0';
    printf("%d %s %s\n", yylineno, "STRING", string_buffer);
    buffer_position = 0;
}

void handle_escape_sequence(){
    bool isInvalidChar = true;
    if(yyleng == 1){
        handle_error(UNDEFINED_ESCAPE_ERROR);
    }
    
    if(yytext[1] == 'x'){
        handle_hex_escape();
        return;
    }
    if(ESCAPE_CHARS.find(yytext[1]) == std::string::npos){
        handle_error(UNDEFINED_ESCAPE_ERROR, yytext+1);
    }
    if(yytext[1] == '0'){
        string_buffer[buffer_position++] = '\0';
    }
    if(yytext[1] == 'n'){
        string_buffer[buffer_position++] = '\n';
    }
    if(yytext[1] == 'r'){
        string_buffer[buffer_position++] = '\r';
    }
    if(yytext[1] == 't'){
        string_buffer[buffer_position++] = '\t';
    }
    if(yytext[1] == '\\'){
        string_buffer[buffer_position++] = '\\';
    }
    if(yytext[1] == '\"'){
        string_buffer[buffer_position++] = '\"';
    }
}

void handle_hex_escape(){
    unsigned int hexValue;
    char *hexChars = yytext+2; 
    std::string hexStr = std::string(hexChars);
    if(yyleng < 4 || hexStr.find_first_not_of(HEX_CHARS) != std::string::npos){
        handle_error(UNDEFINED_ESCAPE_ERROR, yytext+1);
    } 
    
    hexValue = std::stoul(yytext+2, nullptr, 16);
    if(hexValue >= VALID_RANGE_START && hexValue <= VALID_RANGE_END){
        string_buffer[buffer_position++] = static_cast<char>(hexValue);
    } else {
        handle_error(UNDEFINED_ESCAPE_ERROR, yytext+1);
    }
}

void handle_error(int errorType, char* error_seq){
    switch (errorType){
        case UNDEFINED_ESCAPE_ERROR: {
            printf("Error undefined escape sequence %s\n", error_seq);
            break;
        }
        case INVALID_CHAR_ERROR: {
            printf("Error %s\n", error_seq);
            break;
        }
        case UNCLOSED_STRING_ERROR: {
            printf("Error unclosed string\n");
        } 
    }
    exit(0);
}

void print_token(std::string token_name){
  std::cout<<yylineno<<" "<<token_name<<" "<<yytext<<"\n";
}
