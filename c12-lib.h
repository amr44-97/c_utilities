#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <initializer_list>
extern "C" {

struct _IO_FILE;
typedef struct _IO_FILE FILE;

#define SEEK_SET	0	/* Seek from beginning of file.  */
#define SEEK_CUR	1	/* Seek from current position.  */
#define SEEK_END	2	/* Seek from end of file.  */

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

extern int fseek (FILE *__stream, long int __off, int __whence)
  __attribute__ ((__nonnull__ (1)));

extern long int ftell (FILE *__stream) __attribute__ ((__nonnull__ (1)));

extern size_t fread (void *__restrict __ptr, size_t __size,
       size_t __n, FILE *__restrict __stream)
  __attribute__ ((__nonnull__ (4)));

extern int fclose (FILE *__stream) __attribute__ ((__nonnull__ (1)));
extern FILE *fopen (const char *__restrict __filename,
      const char *__restrict __modes)
  __attribute__ ((__malloc__)) ;

extern int fprintf (FILE *__restrict __stream,
      const char *__restrict __format, ...) __attribute__ ((__nonnull__ (1)));

extern int printf (const char *__restrict __format, ...);

extern int sprintf (char *__restrict __s,
      const char *__restrict __format, ...) noexcept (true);
}


typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
typedef size_t usize;

// allocate `n` elements of type `T`.
#define c12_allocate(T, n) (T*)malloc(sizeof(T) * n)

// we will leak some memory here.
// #define temp_alloc(T,n) allocate(T,n)

enum class TokenKind : u32 {
    Eof,
    Identifier,
    TypeNameIdent,
    NumberLiteral,
    StringLiteral,
    CharLiteral,
    WhiteSpace,
    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,
    Period,
    Ellipsis2,
    Ellipsis3,
    Colon,
    ColonEqual,
    ColonColon,
    Equal,
    EqualEqual,
    Semicolon,
    Comma,
    Bang,
    BangEqual,
    Questionmark,
    DollarSign,
    AtSign,
    Hash,
    Plus,
    PlusPlus,
    PlusEqual,
    Minus,
    MinusMinus,
    MinusEqual,
    Arrow,
    Asterisk,
    AsteriskEqual,
    Slash,
    SlashEqual,
    Percent,
    PercentEqual,
    Pipe,
    PipeEqual,
    PipePipe,
    Ampersand,
    AmpersandEqual,
    AmpersandAmpersand,
    Caret,
    CaretEqual,
    Tilde,
    TildeEqual,
    LessThan,
    ShiftLeft,
    ShiftLeftEqual,
    LessOrEqual,
    GreaterThan,
    ShiftRight,      // >>
    ShiftRightEqual, // >>=
    GreaterOrEqual,
    Keyword_return,
    Keyword_const,
    Keyword_let,
    Keyword_static,
    Keyword_if,
    Keyword_else,
    Keyword_for,
    Keyword_while,
    Keyword_do,
    Keyword_goto,
    Keyword_switch,
    Keyword_case,
    Keyword_break,
    Keyword_default,
    Keyword_struct,
    Keyword_enum,
    Keyword_union,
    Keyword_typedef,
    Keyword_sizeof,
    keyword_signed,
    Keyword_unsigned,
    Keyword_int,
    Keyword_bool,
    Keyword_char,
    Keyword_short,
    Keyword_long,
    Keyword_float,
    Keyword_double,
    Keyword_true,
    Keyword_false,
    Keyword_null,
    Keyword_u8,
    Keyword_i8,
    Keyword_u16,
    Keyword_i16,
    Keyword_u32,
    Keyword_i32,
    Keyword_u64,
    Keyword_i64,
    Keyword_f32,
    Keyword_f64,
    Keyword_include,
};

enum class AstKind {
    Root = 0,

    /* Binary Operations */

    // lhs + rhs
    Add,
    // lhs - rhs
    Sub,
    // lhs * rhs
    Mul,
    // lhs / rhs
    Div,
    // lhs % rhs
    Mod,
    // lhs < rhs
    Lt,
    // lhs > rhs
    Gt,
    // lhs <= rhs
    LtEq,
    // lhs >= rhs
    GtEq,
    // lhs == rhs
    EqEq,
    //  lhs != rhs
    NotEq,
    // lhs && rhs
    BoolAnd,
    // lhs || rhs
    BoolOr,
    // lhs | rhs
    BitOr,
    // lhs & rhs
    BitAnd,
    // lhs ^ rhs
    BitXor,
    // lhs << rhs
    Shl,
    // lhs >> rhs
    Shr,

    /* Unary Operations */

    // !rhs
    BoolNot,
    // -rhs
    Neg,
    // ~rhs
    BitNot,
    // &rhs
    AddressOf,
    // *rhs
    Dereference,

    /* Expressions */
    /* Expression Literals */

    Identifier,
    TypeName = Identifier,

    StringLiteral,

    NumberLiteral,

    BooleanLiteral,

    NullLiteral,

    EnumLiteral,

    // lhs(rhs) -> lhs: expr(Identifier), rhs: list(expr)
    Call,
    // lhs.rhs -> lhs: expr, rhs: Identifier
    Member,
    // (lhs) rhs -> lhs: Typeexpr , rhs: expr(Identifier)
    Cast,
    // rhs -> rhs: TypeExpr
    SizeOf,
    // lhs[rhs] -> lhs: expr, rhs: expr
    ArrayAccess,
    // expr { expr = expr, ... }
    StructInit,

    /* statements */
    // expr
    ExprStmt,
    // { `stmt`,`stmt`,...} list of statements
    Block,
    // `return` `optional expr` `;`
    Return,
    // defer stmt;
    Defer,
    //
    CallStmt,
    // `lhs` `AssignOp` `rhs`
    // AssignOp= ['=', '+=', '-=', '/=', '*=', '%=', '|=', '~=', '&=', '^=', '>>=', '<<=']
    Assign,
    //
    TypedVarDecl,
    ConstDecl,
    InferredVarDecl,
    AutoVarDecl,
    // if `expr` `stmt` `optiona<stmt<else_stmt>>`
    IfStmt,
    // While `expr` `stmt<block>`
    WhileLoop,
    // for `expr` in `expr` `stmt<block>`
    ForLoop,
    // type Identifier =? default_value?
    FuncParam,
    // TypedVarDecl | Decl
    StructField,
    /* Top Level */
    FuncDef,
    FuncProto,
    StructDef,
    StructDecl,
    EnumDef,
    UnionDef,
    GlobalVarDecl,
    IncludeStmt,
    MacroDef,
    AliasStmt,
};

struct Location {
    u32 offset;
    u32 len;
    u32 line;
};

struct string {
    char* ptr;
    usize len;

    const char* cstr() const { return ptr; }
    char& operator[](u32 index) { return ptr[index]; }
    const char& operator[](u32 index) const { return ptr[index]; }
};

struct Token {
    TokenKind kind;
    Location loc;
    string buf;
    // string buf(string& source) {
    // 	return source.substr(loc.offset,loc.len);
    // }
    union {
        // index of the Identifier in ParsingContext.str_list;
        // get the symbol by ParsingContext.get(u32 id) -> string_view
        u32 symbol;
        u64 integer;
    } value;
};

struct Symbol {};

#ifndef ARENA_INIT_MEMORY
#define ARENA_INIT_MEMORY 1024 * 256
#endif

// NOTE: this is for temporary allocations

struct C12Arena {
    u8* bytes = nullptr;
    u8* current = nullptr;
    usize capacity = 0;

    constexpr static C12Arena init(usize initial_size = ARENA_INIT_MEMORY) {
        auto a = C12Arena{
            .bytes = (u8*)calloc(initial_size, sizeof(u8)),
            .current = 0,
        };
        a.current = a.bytes;
        a.capacity = initial_size;
        return a;
    }

    template <typename T, usize Size = sizeof(T)> T* alloc(usize elements) {
        if (current >= (bytes + capacity)) {
            fprintf(
                stderr,
                "[Allocator Error]: Temporary allocation exceded allocated memory -512KB- for the arena\n");
            fprintf(stderr, "               to avoid use after free error or heap overflow increase the\n");
            fprintf(stderr, "               arena initial capacity or create a new arena.\n");
            exit(1);
        }
        auto cur = current;
        current += (Size * elements);
        return (T*)cur;
    }

    void deinit() {
        if (bytes != nullptr) {
            free(bytes);
            current = nullptr;
        }
    }

    ~C12Arena() { deinit(); }
};

#ifndef TEMP_MEMORY
#define TEMP_MEMORY 1024 * 256
#endif

static C12Arena __c12_temp_allocator = C12Arena::init(TEMP_MEMORY);

#define temp_alloc(T, _size) __c12_temp_allocator.alloc<T>(_size)

template <typename T, usize Alignment = alignof(T)> struct DynArray {
    static_assert((Alignment & (Alignment - 1)) == 0, "Alignment must be a power of 2");

    void ensure_capacity(usize new_capacity) {
        if (capacity >= new_capacity)
            return;
        usize better_capacity = capacity;

        do {
            better_capacity = better_capacity * 5 / 2 + 8;
        } while (better_capacity < new_capacity);

        const usize aligned_size = ((better_capacity * sizeof(T)) + Alignment - 1) & ~(Alignment - 1);
        ptr = (T*)realloc(ptr, aligned_size);
        capacity = better_capacity;
    }

    void destroy() {
        assert(ptr != nullptr);
        std::free(ptr);
        len = capacity = 0;
    }

    u32 append(T elem) {
        if (capacity <= len + 1)
            ensure_capacity(len + 1);
        ptr[len++] = elem;
        return len - 1;
    }

    void resize(usize new_len) {
        ensure_capacity(new_len);
        len = new_len;
    }

    void append(std::initializer_list<T> items) { append_items(items.begin(), items.size()); }

    void append_items(const T* items, size_t count) {
        for (int i = 0; i < count; i++) {
            append(items[i]);
        }
    }

    const T& last() const {
        assert(len >= 1);
        return ptr[len - 1];
    }

    T& last() {
        assert(len >= 1);
        return ptr[len - 1];
    }

    T* begin() { return ptr; }
    T* end() { return ptr + len; }

    T& operator[](usize i) const {
        assert(i < len && "access beyond the lenght of the array");
        return ptr[i];
    }

    T operator[](usize i) {
        assert(i < len && "access beyond the lenght of the array");
        return ptr[i];
    }

    T* ptr = nullptr;
    u32 len = 0;
    u32 capacity = 0;
};

template <typename T> auto create_dyn(usize _size = 8) {
    DynArray<T> l = {.ptr = nullptr, .len = 0, .capacity = 0};
    l.ensure_capacity(_size);
    return l;
}

#define DynArray(T) DynArray<T>

template <typename T> struct slice {
    T* ptr;
    u32 len;

    constexpr slice() = default;
    constexpr slice(T* _Data, u32 _length) {
        ptr = _Data;
        len = _length;
    }
    constexpr const T& operator[](u32 _index) const { return ptr[_index]; }
    constexpr T& operator[](u32 _index) { return ptr[_index]; }

    constexpr u32 length() const { return len; }

    // Add these to make it a standard range:
    constexpr T* begin() { return ptr; }
    constexpr T* end() { return ptr + len; }

    constexpr const T* begin() const { return ptr; }
    constexpr const T* end() const { return ptr + len; }
};

// 1. Open the std namespace to specialize the formatter

struct File {
    const char* name;
    string content;

    const char& operator[](usize _index) const { return content[_index]; }
};

const char* to_str(TokenKind kind);
const char* to_str(AstKind kind);

constexpr File read_file(const char* file_name);

struct Arena {
    u8* bytes = nullptr;
    u8* current = nullptr;
    usize capacity = 0;

    template <typename T, usize Size = sizeof(T)> T* alloc(usize elements) {
        if (current >= (bytes + capacity)) {
            fprintf(stderr,
                    "[Arena Error]: arena allocation exceded allocated memory -256KB- for the arena\n");
            fprintf(stderr, "               to avoid use after free error or heap overflow increase the\n");
            fprintf(stderr, "               arena initial capacity or create a new arena.\n");
            exit(1);
        }
        auto _cur = current;
        current += (Size * elements);
        return (T*)_cur;
    }

    void deinit() {
        if (bytes != nullptr) {
            free(bytes);
            current = nullptr;
        }
    }
};

constexpr Arena create_arena(usize initial_size = ARENA_INIT_MEMORY) {
    auto a = Arena{
        .bytes = (u8*)calloc(initial_size, sizeof(u8)),
        .current = 0,
    };
    a.current = a.bytes;
    a.capacity = initial_size;
    return a;
}

enum class Ok: u8 {
	NullOpt,
	ErrorResult,
	Done,
};

template <typename T> struct
[[nodiscard]] Optional {
  private:
    T* _M_value = nullptr;
    bool checked = false;

  public:

    constexpr Ok ok() {
        checked = true;
		if(_M_value != nullptr)
			return Ok::Done;
		return Ok::NullOpt;
	}

    Optional() = default;
    Optional(T _value) {
        _M_value = temp_alloc(T, 1);
        *_M_value = _value;
    }

    Optional<T>& set(T _value) {
        _M_value = temp_alloc(T, 1);
        *_M_value = _value;
        return *this;
    }

    constexpr T unwrap() const {
        if (!checked && _M_value != nullptr) {
            fprintf(stderr, "[panic]: Optional value unchcked\n");
            fprintf(stderr, "         %s() from  %s:%d\n", __func__, __FILE_NAME__, __LINE__);
            exit(1);
        }

        return *_M_value;
    }

    bool operator==(void* p) { return (void*)_M_value == p; }

   constexpr operator bool() {  
	   switch(ok()){
		   case Ok::Done: return true;
		   default: return false;
	   } 
   }

};

struct Error {
    Error() = default;
    Error(const char* _err) { name = _err; }
    const char* name;
};

template <typename T> struct ErrorUnion {
    ErrorUnion() = default;
    ErrorUnion(Error __error) : error(__error) {}
    ErrorUnion(T _value) {
        _M_value = temp_alloc(T, 1);
        *_M_value = _value;
    }

  private:
    union {
        T* _M_value;
        Error error;
    };

    template <typename Tp> friend struct Result;
};

template <typename T> struct 
[[nodiscard]] Result : ErrorUnion<T> {
    Result() = default;
    Result(T _value) : ErrorUnion<T>(_value) { valid = true; }
    Result(Error __error) : ErrorUnion<T>(__error) { valid = false; }

    constexpr T& unwrap() const {
        if (valid) {
            return *this->_M_value;
        }
        fprintf(stderr, "[panic: Result<T>]: %s at (%s:%d) `%s`\n", error_name(), __FILE_NAME__, __LINE__,
                __PRETTY_FUNCTION__);
        exit(1);
    }

     constexpr operator bool() const { return valid; }

    constexpr operator T() const {
        if (!valid)
            unwrap();

        return *this->_M_value;
    }

	// ok() and value() are used for the Some macro
	constexpr Ok ok() const {
		if(!valid)
			return Ok::ErrorResult;
	
		return Ok::Done;
	}



    constexpr const char* error_name() const {
        if (!valid) {
            return this->error.name;
        }
        return nullptr;
    }

  private:
    bool valid = false;
};



#define Some(_VAR_) ({\
	switch (_VAR_.ok()) {                                                                               \
		case Ok::ErrorResult:{ 																		    \
			fprintf(stderr,"[panic]: Error return from Result<T> with `%s` [%s:%d] called from `%s`\n",#_VAR_,__FILE_NAME__,__LINE__,__PRETTY_FUNCTION__);					\
			_VAR_.unwrap(); /* unwraps the error to output */	    		             				\
			exit(1); 																					\
		 }break; 																						\
		case Ok::NullOpt:{ 																				\
			fprintf(stderr,"[panic]: Dereferencing a Null Optional `%s`\n",#_VAR_);						\
			fprintf(stderr,"         [%s:%d] -> %s\n",__FILE_NAME__,__LINE__,__PRETTY_FUNCTION__);		\
			exit(1); 																					\
		} 																								\
		case Ok::Done: break; 																			\
	}                                                                                                   \
	_VAR_.unwrap(); 																					\
})


template<typename Fn> 
struct DeferGuard {
	Fn func;

	~DeferGuard(){
		func();
	}
};


#define __pp_concat(x,y) x##y
#define __pp_defer_concat(x,y) __pp_concat(x,y)

#define defer(code)                                                                     \
	auto __pp_defer_concat(__defer_var_,__COUNTER__) = DeferGuard{ [&](){     \
		static_assert(true, "ensure scope validitiy" );                                 \
		code; 																			\
	} };


/**************** Functions ********************/

constexpr File read_file(const char* file_name) {
    FILE* fp = fopen(file_name, "r+");
    if (!fp) {
        fprintf(stderr, "[Lexing Error]: failed to open file `%s`, at {%s: %s : %d}\n", file_name,
                __FILE_NAME__, __func__, __LINE__);
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    size_t fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    auto f = File{.name = file_name, .content = {nullptr, 0}};
    f.content.ptr = temp_alloc(char, fsize + 1);

    size_t n = fread(f.content.ptr, sizeof(char), fsize, fp);

    if (n != fsize) {
        fprintf(stderr, "[Lexing Error]: expected %lu bytes from file `%s` but got %lu bytes\n", fsize,
                file_name, n);
        exit(1);
    }
    fclose(fp);
    return f;
}
