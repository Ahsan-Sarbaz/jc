#pragma once
#include <string_view>

enum class TokenType
{
	None,
	Identifier,
	Number,
	StringLiteral,

	// operators
	Plus,
	PlusEqual,
	Minus,
	MinusEqual,
	Star,
	StarStar,
	StarEqual,
	Slash,
	SlashEqual,
	Modulo,
	ModuloEqual,
	Equal,
	EqualEqual,
	NotEqual,
	LessThan,
	LessThanEqual,
	GreaterThan,
	GreaterThanEqual,
	And,
	Or,
	Not,
	BitAnd,
	BitOr,
	BitXor,
	BitNot,

	BackSlash,
	Comma,
	Colon,
	LeftParen,
	RightParen,
	LeftBrace,
	RightBrace,
	Dot,
	SemiColon,

	// keywords
	Function,
	Continue,
	Break,
	Let,
	True,
	False,
	If,
	Else,
	Return,
	For,
	Struct,
	Null,
	Extern,
	Cpp,

	// reserved types
	I8,
	I16,
	I32,
	I64,
	U8,
	U16,
	U32,
	U64,
	F32,
	F64,
	Char,
	Bool,
	Str,
	Void,

	EndOfFile
};

struct Token
{
	TokenType type;
	std::string_view value;
	size_t line;
	size_t column;

	explicit Token(TokenType type, const std::string_view& value, size_t line, size_t column)
		: type(type)
		, value(value)
		, line(line)
		, column(column)
	{
	}
};