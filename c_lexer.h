#ifndef C_LEXER_H
#define C_LEXER_H
#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>

#ifdef  __cplusplus
extern "C" {
#endif

jmp_buf lex_err; 

typedef enum LexerError {
    Error_unhandled_char = 19,
    Error_string_literal_no_end_quote = 20,
}LexerError ;

typedef enum TokenKind : uint32_t {
    Tok_eof,
    Tok_identifier,
    Tok_number_literal,
    Tok_string_literal,
    Tok_char_literal,

    Tok_l_paren,
    Tok_r_paren,
    Tok_l_brace,
    Tok_r_brace,
    Tok_l_bracket,
    Tok_r_bracket,
    Tok_period,
    Tok_ellipsis2,
    Tok_ellipsis3,
    Tok_colon,
    Tok_colon_equal,
    Tok_colon_colon,
    Tok_equal,
    Tok_equal_equal,
    Tok_semicolon,
    Tok_comma,
    Tok_bang,
    Tok_bang_equal,
    Tok_questionmark,
    Tok_dollar_sign,
    Tok_at_sign,
    Tok_hash,
    Tok_plus,
    Tok_plus_plus,
    Tok_plus_equal,
    Tok_minus,
    Tok_minus_minus,
    Tok_minus_equal,
    Tok_arrow,
    Tok_asterisk,
    Tok_asterisk_equal,
    Tok_slash,
    Tok_slash_equal,
    Tok_percent,
    Tok_percent_equal,
    Tok_pipe,
    Tok_pipe_equal,
    Tok_pipe_pipe,
    Tok_ampersand,
    Tok_ampersand_equal,
    Tok_ampersand_ampersand,
    Tok_caret,
    Tok_caret_equal,
    Tok_tilde,
    Tok_tilde_equal,
    Tok_angle_bracket_left,
    Tok_angle_bracket_left_left,
    Tok_angle_bracket_left_left_equal,
    Tok_angle_bracket_left_equal,
    Tok_angle_bracket_right,
    Tok_angle_bracket_right_right,
    Tok_angle_bracket_right_right_equal,
    Tok_angle_bracket_right_equal,

    Tok_keyword_return,
    Tok_keyword_const,
    Tok_keyword_let,
    Tok_keyword_static,
    Tok_keyword_if,
    Tok_keyword_else,
    Tok_keyword_for,
    Tok_keyword_while,
    Tok_keyword_do,
    Tok_keyword_goto,
    Tok_keyword_switch,
    Tok_keyword_case,
    Tok_keyword_break,
    Tok_keyword_default,
    Tok_keyword_struct,
    Tok_keyword_enum,
    Tok_keyword_union,
    Tok_keyword_typedef,
    Tok_keyword_sizeof,
    Tok_Keyword_signed,
    Tok_keyword_unsigned,
    Tok_keyword_int,
    Tok_keyword_bool,
    Tok_keyword_char,
    Tok_keyword_short,
    Tok_keyword_long,
    Tok_keyword_float,
    Tok_keyword_double,
    Tok_keyword_true,
    Tok_keyword_false,
    Tok_keyword_u8,
    Tok_keyword_i8,
    Tok_keyword_u16,
    Tok_keyword_i16,
    Tok_keyword_u32,
    Tok_keyword_i32,
    Tok_keyword_u64,
    Tok_keyword_i64,
    Tok_keyword_f32,
    Tok_keyword_f64,

    Tok_builtin_include,
    Tok_builtin_embed,
    Tok_builtin_define,
    Tok_builtin_ifdef,
    Tok_builtin_ifndef,
    Tok_builtin_endif,
}TokenKind;


#define BUILTINS_TABLE_LEN 6 
static  struct {const char* name; TokenKind kind;} BuiltinsTable[BUILTINS_TABLE_LEN] = {

    {"include" ,Tok_builtin_include},
    {"embed"  , Tok_builtin_embed},
    {"define" , Tok_builtin_define},
    {"ifdef"  , Tok_builtin_ifdef},
    {"ifndef" , Tok_builtin_ifndef},
    {"endif"  , Tok_builtin_endif},
};

#define KEYWORDS_TABLE_LEN 40
static  struct {const char* name; TokenKind kind;} KeywordsTable[KEYWORDS_TABLE_LEN] = {
    { "return"    , Tok_keyword_return },
    { "const"     , Tok_keyword_const },
    { "let"       , Tok_keyword_let },
    { "static"    , Tok_keyword_static },
    { "if"        , Tok_keyword_if },
    { "else"      , Tok_keyword_else },
    { "for"       , Tok_keyword_for },
    { "while"     , Tok_keyword_while },
    { "do"        , Tok_keyword_do },
    { "goto"      , Tok_keyword_goto },
    { "switch"    , Tok_keyword_switch },
    { "case"      , Tok_keyword_case },
    { "break"     , Tok_keyword_break },
    { "default"   , Tok_keyword_default },
    { "struct"    , Tok_keyword_struct },
    { "enum"      , Tok_keyword_enum },
    { "union"     , Tok_keyword_union },
    { "typedef"   , Tok_keyword_typedef },
    { "sizeof"    , Tok_keyword_sizeof },
    { "signed"    , Tok_Keyword_signed },
    { "unsigned"  , Tok_keyword_unsigned },
    { "int"       , Tok_keyword_int },
    { "bool"       , Tok_keyword_bool },
    { "char"      , Tok_keyword_char },
    { "short"     , Tok_keyword_short },
    { "long"      , Tok_keyword_long },
    { "float"     , Tok_keyword_float },
    { "double"    , Tok_keyword_double },
    { "true"      , Tok_keyword_true },
    { "false"     , Tok_keyword_false },
    { "u8"        , Tok_keyword_u8 },
    { "i8"        , Tok_keyword_i8 },
    { "u16"       , Tok_keyword_u16 },
    { "i16"       , Tok_keyword_i16 },
    { "u32"       , Tok_keyword_u32 },
    { "i32"       , Tok_keyword_i32 },
    { "u64"       , Tok_keyword_u64 },
    { "i64"       , Tok_keyword_i64 },
    { "f32"       , Tok_keyword_f32 },
    { "f64"       , Tok_keyword_f64 },
};

typedef enum LexingState{
    Lexing_start,
    Lexing_identifier,
    Lexing_number_literal,
    Lexing_string_literal,
    Lexing_char_literal,
    Lexing_equal,
    Lexing_plus,
    Lexing_minus,
    Lexing_asterisk,
    Lexing_slash,
    Lexing_percent,
    Lexing_colon,
    Lexing_tilde,
    Lexing_caret,
    Lexing_ampersand,
    Lexing_angle_bracket_left,
    Lexing_angle_bracket_right,
    Lexing_pipe,
    Lexing_bang,
    Lexing_period,
    Lexing_single_line_comment,
    Lexing_builtin,
}LexingState;

typedef struct CFile {
    const char* name;
    size_t size;
    FILE* fp;
    char* buffer; // buffer containing file content;
} CFile;

typedef struct Location {
    uint32_t offset;
    uint32_t len;
    uint32_t line;
} Location;

typedef struct Token {
    TokenKind kind;
    Location loc;
    char* buf;
} Token; 

typedef struct Lexer {
    const char* source;
    uint32_t src_len;
    uint32_t index;
    uint32_t line;
} Lexer;

CFile cfile_init_alloc(const char* file_name);
void cfile_deinit(CFile* file);
Token create_token(Lexer* lexer,TokenKind kind,uint32_t start,uint32_t end);
Lexer lexer_init(const char* source);
Lexer lexer_init_s(const char* source,uint32_t src_len);
Token lexer_next_token(Lexer* lexer);
const char* token_buf_noalloc(const char* source,Token* tok);
const char* lexer_get_line(Lexer* lexer, Token* token);
const char* token_get_line(const char* source, Token* token);


#define IMPEL_C_LEXER
#ifdef IMPEL_C_LEXER

Lexer lexer_init_s(const char* source, uint32_t src_len){
    return (Lexer){
        .source = source,
        .src_len = src_len,
        .index = 0,
        .line = 1,
    };
}

Lexer lexer_init(const char* source){
    return lexer_init_s(source, strlen(source));
}

Token lexer_next_token(Lexer* lex) { 
        LexingState state = Lexing_start;
        Token result = (Token){
            .kind = Tok_eof,
            .loc = (Location){
                .offset = lex->index,
                .len = 1, /*undefined*/
                .line = lex->line, 
            },
        };
loop: switch(state){
        case Lexing_start:{
             switch(lex->source[lex->index]){
                 case '\0':
                     result.kind = Tok_eof;
                     goto end;
                 case ' ':
                 case '\t':
                     lex->index += 1;
                     result.loc.offset = lex->index;
                     goto loop;
                 
                 case '\n':
                     lex->line += 1;
                     lex->index += 1;
                     result.loc.offset = lex->index;
                     goto loop;
                
                 case '#':
                     lex->index += 1;
                     result.kind = Tok_hash;
                     result.loc.offset = lex->index;
                     state = Lexing_builtin;
                     goto loop;
 
                 case 'a'...'z':
                 case 'A'...'Z':
                 case '_':
                     result.kind = Tok_identifier;
                     state = Lexing_identifier;
                     goto loop;
 
                 case '\'':
                     lex->index += 1;
                     result.kind = Tok_char_literal;
                     state = Lexing_char_literal;
                     goto loop;

                 case '"':
                     lex->index += 1;
                     result.kind = Tok_string_literal;
                     state = Lexing_string_literal;
                     goto loop;

                 case '0'...'9':
                     result.kind = Tok_number_literal;
                     state = Lexing_number_literal;
                     goto loop;
                
                 case '=':
                     result.kind = Tok_equal;
                     state = Lexing_equal;
                     lex->index += 1;
                     goto loop;

                case '+':
                     result.kind = Tok_plus;
                     state = Lexing_plus;
                     lex->index += 1;
                     goto loop;

                case '-':
                     result.kind = Tok_minus;
                     state = Lexing_minus;
                     lex->index += 1;
                     goto loop;

                case '*':
                     result.kind = Tok_asterisk;
                     state = Lexing_asterisk;
                     lex->index += 1;
                     goto loop;
 
                case '/':
                     result.kind = Tok_slash;
                     state = Lexing_slash;
                     lex->index += 1;
                     goto loop;
                
                case '%':
                     result.kind = Tok_percent;
                     state = Lexing_percent;
                     lex->index += 1;
                     goto loop;

                case ':':
                     result.kind = Tok_colon;
                     state = Lexing_colon;
                     lex->index += 1;
                     goto loop;

                case '.':
                     result.kind = Tok_period;
                     state = Lexing_period;
                     lex->index += 1;
                     goto loop;


                case '~':
                     result.kind = Tok_tilde;
                     state = Lexing_tilde;
                     lex->index += 1;
                     goto loop;

                case '^':
                     result.kind = Tok_caret;
                     state = Lexing_caret;
                     lex->index += 1;
                     goto loop;

                case '&':
                     result.kind = Tok_ampersand;
                     state = Lexing_ampersand;
                     lex->index += 1;
                     goto loop;

                case '<':
                     result.kind = Tok_angle_bracket_left;
                     state = Lexing_angle_bracket_left;
                     lex->index += 1;
                     goto loop;

                case '>':
                     result.kind = Tok_angle_bracket_right;
                     state = Lexing_angle_bracket_right;
                     lex->index += 1;
                     goto loop;

                case '|':
                     result.kind = Tok_pipe;
                     state = Lexing_pipe;
                     lex->index += 1;
                     goto loop;

                case '!':
                     result.kind = Tok_bang;
                     state = Lexing_bang;
                     lex->index += 1;
                     goto loop;

                case ';':
                     result.kind = Tok_semicolon;
                     lex->index += 1;
                     goto end;

                case ',':
                     result.kind = Tok_comma;
                     lex->index += 1;
                     goto end;

                case '$':
                     result.kind = Tok_dollar_sign;
                     lex->index += 1;
                     goto end;

                case '@':
                     result.kind = Tok_at_sign;
                     lex->index += 1;
                     goto end;

                case '?':
                     result.kind = Tok_questionmark;
                     lex->index += 1;
                     goto end;

                case '{':
                     result.kind = Tok_l_brace;
                     lex->index += 1;
                     goto end;

                case '}':
                     result.kind = Tok_r_brace;
                     lex->index += 1;
                     goto end;

                case '[':
                     result.kind = Tok_l_bracket;
                     lex->index += 1;
                     goto end;

                case ']':
                     result.kind = Tok_r_bracket;
                     lex->index += 1;
                     goto end;

                case '(':
                     result.kind = Tok_l_paren;
                     lex->index += 1;
                     goto end;

                case ')':
                     result.kind = Tok_r_paren;
                     lex->index += 1;
                     goto end;

                default:
                     fprintf(stderr , "[Lexing Error]: unhandled char `%c`\n",lex->source[lex->index]);
                     longjmp(lex_err, Error_unhandled_char);
             }
        }break;
        case Lexing_identifier:{
             switch (lex->source[lex->index]) {
                 case 'a' ... 'z':
                 case 'A' ... 'Z':
                 case '0' ... '9':
                 case '_':
                     lex->index += 1;
                     goto loop;
                 default:
                     for(int i = 0; i < KEYWORDS_TABLE_LEN ; i++ ){
                         if(!memcmp(&lex->source[result.loc.offset], KeywordsTable[i].name , lex->index - result.loc.offset)){
                             result.kind = KeywordsTable[i].kind;
                         }
                     }
                     goto end;
             }
         }break;
        case Lexing_string_literal:{
             switch (lex->source[lex->index]) {
                case '\\':
                     lex->index += 2;
                     goto loop;
                 case '"':
                     lex->index += 1;
                     goto end;
                case '\n':
                     lex->line += 1;
                     lex->index += 1;
                     goto loop;
                case '\0':
                     fprintf(stderr , "[Lexing Error]: string literal misses `\"`, stuck at eof \n");
                     longjmp(lex_err, Error_string_literal_no_end_quote);
                 default:
                     lex->index += 1;
                     goto loop;
             }
        }break;

        case Lexing_char_literal:{
             switch (lex->source[lex->index]) {
                 case '\'':
                     lex->index += 1;
                     goto end;
                case '\\':
                     lex->index += 2;
                     goto loop;
                 default:
                     lex->index += 1;
                     goto loop;
             }
        }break;

        case Lexing_number_literal:{
             switch (lex->source[lex->index]) {
                 case '0' ... '9':
                     lex->index += 1;
                     goto loop;
                 default:
                     goto end;
             }
        }break;

        case Lexing_equal:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_equal_equal;
                     goto end;
                 default:
                     goto end;
             }
        }break;

        case Lexing_plus:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_plus_equal;
                     goto end;

                 case '+':
                     lex->index += 1;
                     result.kind = Tok_plus_plus;
                     goto end;
                 default:
                     goto end;
             }
        }break;

        case Lexing_minus:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_minus_equal;
                     goto end;

                 case '-':
                     lex->index += 1;
                     result.kind = Tok_minus_minus;
                     goto end;

                 case '>':
                     lex->index += 1;
                     result.kind = Tok_arrow;
                     goto end;

                 default:
                     goto end;
             }
        }break;

        case Lexing_asterisk:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_asterisk_equal;
                     goto end;
                 default:
                     goto end;
             }
        }break;

        case Lexing_slash:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_slash_equal;
                     goto end;
                 case '/':
                     lex->index += 1;
                     state = Lexing_single_line_comment;
                     goto loop;
                 default:
                     goto end;
             }
        }break;

        case Lexing_tilde:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_tilde_equal;
                     goto end;
                 default:
                     goto end;
             }
        }break;

        case Lexing_caret:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_caret_equal;
                     goto end;
                 default:
                     goto end;
             }
        }break;

        case Lexing_ampersand:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_ampersand_equal;
                     goto end;

                 case '&':
                     lex->index += 1;
                     result.kind = Tok_ampersand_ampersand;
                     goto end;
                 default:
                     goto end;
             }
        }break;

        case Lexing_pipe:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_pipe_equal;
                     goto end;

                 case '|':
                     lex->index += 1;
                     result.kind = Tok_pipe_pipe;
                     goto end;

                 default:
                     goto end;
             }
        }break;

        case Lexing_bang:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_bang_equal;
                     goto end;

                 default:
                     goto end;
             }
        }break;

        case Lexing_percent:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_percent_equal;
                     goto end;
                 default:
                     goto end;
             }
        }break;

        case Lexing_angle_bracket_left:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_angle_bracket_left_equal;
                     goto end;

                 case '<':
                     lex->index += 1;
                     result.kind = Tok_angle_bracket_left_left;
                     if(lex->source[lex->index] == '='){
                        result.kind = Tok_angle_bracket_left_left_equal;
                        lex->index += 1;

                     }
                     goto end;

                 default:
                     goto end;
             }
        }break;

        case Lexing_angle_bracket_right:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_angle_bracket_right_equal;
                     goto end;

                 case '<':
                     lex->index += 1;
                     result.kind = Tok_angle_bracket_right_right;
                     if(lex->source[lex->index] == '='){
                        result.kind = Tok_angle_bracket_right_right_equal;
                        lex->index += 1;
                     }
                     goto end;

                 default:
                     goto end;
             }
        }break;

     case Lexing_single_line_comment:{
             switch (lex->source[lex->index]) {
                 case '\n':
                     lex->line++;
                     lex->index += 1;
                     result.loc.offset = lex->index;
                     state = Lexing_start;
                     goto loop;
                 case '\0':
                     lex->index += 1;
                     result.loc.offset = lex->index;
                     result.kind = Tok_eof;
                     goto end;
                 default:
                     lex->index += 1;
                     result.loc.offset = lex->index;
                     goto loop;
             }
        }break;

        case Lexing_colon:{
             switch (lex->source[lex->index]) {
                 case '=':
                     lex->index += 1;
                     result.kind = Tok_colon_equal;
                     goto end;
                 case ':':
                     lex->index += 1;
                     result.kind = Tok_colon_colon;
                     goto end;
                 default:
                     goto end;
             }
        }break;

        case Lexing_period:{
             switch (lex->source[lex->index]) {
                 case '.':
                     result.kind = Tok_ellipsis2;
                     lex->index += 1;
                     
                     if(lex->source[lex->index] == '.') {
                        result.kind = Tok_ellipsis3;
                        lex->index += 1;
                     }
                     goto end;
                 default:
                     goto end;
             }
        }break;

        case Lexing_builtin:{
             switch (lex->source[lex->index]) {
                 case 'a' ... 'z':
                 case 'A' ... 'Z':
                 case '_':
                     lex->index += 1;
                     goto loop;
                 default:
                     for(int i = 0; i < BUILTINS_TABLE_LEN ; i++ ){
                         if(!memcmp(&lex->source[result.loc.offset], BuiltinsTable[i].name , lex->index - result.loc.offset)){
                             result.kind = BuiltinsTable[i].kind;
                         }
                     }
                     goto end;
             }
         }break;

      } // end switch

end:
    result.loc.len = lex->index - result.loc.offset;
    return result;
}


const char* token_enum_to_str(TokenKind kind){
    switch (kind) {
        case Tok_eof: return "eof";
        case Tok_identifier: return "identifier";
        case Tok_number_literal: return "number_literal";
        case Tok_string_literal: return "string_literal";
        case Tok_char_literal: return "char_literal";
        case Tok_l_paren: return "l_paren";
        case Tok_r_paren: return "r_paren";
        case Tok_l_brace: return "l_brace";
        case Tok_r_brace: return "r_brace";
        case Tok_l_bracket: return "l_bracket";
        case Tok_r_bracket: return "r_bracket";
        case Tok_period: return "period";
        case Tok_ellipsis2: return "ellipsis2";
        case Tok_ellipsis3: return "ellipsis3";
        case Tok_colon: return "colon";
        case Tok_colon_equal: return "colon_equal";
        case Tok_colon_colon: return "colon_colon";
        case Tok_equal: return "equal";
        case Tok_equal_equal: return "equal_equal";
        case Tok_semicolon: return "semicolon";
        case Tok_comma: return "comma";
        case Tok_bang: return "bang";
        case Tok_bang_equal: return "bang_equal";
        case Tok_questionmark: return "questionmark";
        case Tok_dollar_sign: return "dollar_sign";
        case Tok_at_sign: return "at_sign";
        case Tok_hash: return "hash";
        case Tok_plus: return "plus";
        case Tok_plus_plus: return "plus_plus";
        case Tok_plus_equal: return "plus_equal";
        case Tok_minus: return "minus";
        case Tok_minus_minus: return "minus_minus";
        case Tok_minus_equal: return "minus_equal";
        case Tok_arrow: return "arrow";
        case Tok_asterisk: return "asterisk";
        case Tok_asterisk_equal: return "asterisk_equal";
        case Tok_slash: return "slash";
        case Tok_slash_equal: return "slash_equal";
        case Tok_percent: return "percent";
        case Tok_percent_equal: return "percent_equal";
        case Tok_pipe: return "pipe";
        case Tok_pipe_equal :return "pipe_equal";
        case Tok_pipe_pipe: return "pipe_pipe";
        case Tok_ampersand: return "ampersand";
        case Tok_ampersand_equal: return "ampersand_equal";
        case Tok_ampersand_ampersand: return "ampersand_ampersand";
        case Tok_caret: return "caret";
        case Tok_caret_equal: return "caret_equal";
        case Tok_tilde: return "tilde";
        case Tok_tilde_equal: return "tilde_equal";
        
        case Tok_angle_bracket_left: return "angle_bracket_left";
        case Tok_angle_bracket_left_left: return "angle_bracket_left_left";
        case Tok_angle_bracket_left_left_equal: return "angle_bracket_left_left_equal";
        case Tok_angle_bracket_left_equal: return "angle_bracket_left_equal";
        
        case Tok_angle_bracket_right: return "angle_bracket_right";
        case Tok_angle_bracket_right_right: return "angle_bracket_right_right";
        case Tok_angle_bracket_right_right_equal: return "angle_bracket_right_right_equal";
        case Tok_angle_bracket_right_equal: return "angle_bracket_right_equal";
        
        case Tok_keyword_return: return "keyword_return";
        case Tok_keyword_const: return "keyword_const";
        case Tok_keyword_let: return "keyword_let";
        case Tok_keyword_static: return "keyword_static";
        case Tok_keyword_if: return "keyword_if";
        case Tok_keyword_else: return "keyword_else";
        case Tok_keyword_for: return "keyword_for";
        case Tok_keyword_while: return "keyword_while";
        case Tok_keyword_do: return "keyword_do";
        case Tok_keyword_goto: return "keyword_goto";
        case Tok_keyword_switch: return "keyword_switch";
        case Tok_keyword_case: return "keyword_case";
        case Tok_keyword_break: return "keyword_break";
        case Tok_keyword_default: return "keyword_default";
        case Tok_keyword_struct: return "keyword_struct";
        case Tok_keyword_enum: return "keyword_enum";
        case Tok_keyword_union: return "keyword_union";
        case Tok_keyword_typedef: return "keyword_typedef";
        case Tok_keyword_sizeof: return "keyword_sizeof";
        case Tok_Keyword_signed: return "Keyword_signed";
        case Tok_keyword_unsigned: return "keyword_unsigned";
        case Tok_keyword_int: return "keyword_int";
        case Tok_keyword_bool: return "keyword_bool";
        case Tok_keyword_char: return "keyword_char";
        case Tok_keyword_short: return "keyword_short";
        case Tok_keyword_long: return "keyword_long";
        case Tok_keyword_float: return "keyword_float";
        case Tok_keyword_double: return "keyword_double";
        case Tok_keyword_true: return "keyword_true";
        case Tok_keyword_false: return "keyword_false";
        case Tok_keyword_u8: return "keyword_u8";
        case Tok_keyword_i8: return "keyword_i8";
        case Tok_keyword_u16: return "keyword_u16";
        case Tok_keyword_i16: return "keyword_i16";
        case Tok_keyword_u32: return "keyword_u32";
        case Tok_keyword_i32: return "keyword_i32";
        case Tok_keyword_u64: return "keyword_u64";
        case Tok_keyword_i64: return "keyword_i64";
        case Tok_keyword_f32: return "keyword_f32";
        case Tok_keyword_f64: return "keyword_f64";
        case Tok_builtin_include: return "builtin_include";
        case Tok_builtin_embed: return "builtin_embed";
        case Tok_builtin_define: return "builtin_define";
        case Tok_builtin_ifdef: return "builtin_ifdef";
        case Tok_builtin_ifndef: return "builtin_ifndef";
        case Tok_builtin_endif: return "builtin_endif";
    }
    return "Error: Unknown enum kind";
}


#ifdef __cplusplus
}
#endif
#endif



CFile cfile_init_alloc(const char* file_name){
    CFile f = {0};
    f.name = file_name;
    f.fp = fopen(file_name, "r");
 
    if(!f.fp){
        fprintf(stderr,"[Lexing Error]: failed to open file `%s`, at {%s: %s : %d}\n",file_name,__FILE_NAME__, __func__,__LINE__);
        exit(1);
    }

    fseek(f.fp, 0 , SEEK_END);
    f.size = ftell(f.fp);
    fseek(f.fp, 0, SEEK_SET);
 
    f.buffer = calloc(f.size + 1, sizeof(char));
    
    if(!f.buffer) {
        fprintf(stderr, "[Lexing Error]: failed to allocate memory buffer for file `%s`\n",file_name);
        exit(1);
    }
    
    size_t n = fread(f.buffer, sizeof(char), f.size , f.fp);
    
    if(n != f.size) {
        fprintf(stderr , "[Lexing Error]: expected %lu bytes from file `%s` but got %lu bytes\n",f.size,file_name,n);
        exit(1);
    }
    return  f;
}

void cfile_deinit(CFile* file){
    free(file->buffer);
    fclose(file->fp);
   *file = (CFile){0};
}

const char* token_buf_noalloc(const char* source,Token* tok){
    static char tok_buf[1024 * 10];
    memset(tok_buf, 0, 1024 * 10);
    memcpy(tok_buf, &source[tok->loc.offset],tok->loc.len);
    return tok_buf;
}


#endif

#if 0
int main(int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr,"usage: %s <FILE_NAME>\n",argv[0]);
        exit(1);
    }
    
    switch (setjmp(lex_err)) {
        case Error_unhandled_char:
            goto end;
            break;
        case Error_string_literal_no_end_quote:
            goto end;
            break;
    }

   CFile file = cfile_init_alloc(argv[1]);
    printf("%s\n",file.buffer);
   Lexer lex =  lexer_init(file.buffer);
    
   while(true){
        Token tok =  lexer_next_token(&lex);
        printf("%s => %s\n",token_enum_to_str(tok.kind),token_buf_noalloc(lex.source, &tok));
        if(tok.kind == Tok_eof) break;
   }
end:
   cfile_deinit(&file);
}
#endif
