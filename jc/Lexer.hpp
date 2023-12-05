#pragma once
#include "Token.hpp"
#include "Context.hpp"
#include <vector>
#include <unordered_map>


class Lexer {
public:
	explicit Lexer(Context*);

	void Lex();

private:
	void SkipWhitespace();
	void SkipToNextLine();
	char Peek(int offset = 0);
	char Eat();

	std::string_view LexNumber();
	bool IsNumber(char c);


	size_t line;
	size_t column;
	Context* context;
	size_t cursor = 0;

	std::unordered_map<std::string_view, TokenType> keywords;
};