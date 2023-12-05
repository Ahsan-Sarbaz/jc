#include "Lexer.hpp"
#include <string>

Lexer::Lexer(Context* context)
	:context(context)
	, cursor(0)
	, line(1)
	, column(0)

{
	keywords["fn"] = TokenType::Function;
	keywords["continue"] = TokenType::Continue;
	keywords["break"] = TokenType::Break;
	keywords["let"] = TokenType::Let;
	keywords["true"] = TokenType::True;
	keywords["false"] = TokenType::False;
	keywords["if"] = TokenType::If;
	keywords["else"] = TokenType::Else;
	keywords["return"] = TokenType::Return;
	keywords["for"] = TokenType::For;
	keywords["struct"] = TokenType::Struct;
	keywords["null"] = TokenType::Null;
	keywords["extern"] = TokenType::Extern;
	keywords["cpp"] = TokenType::Cpp;

	// reserved types
	keywords["i8"] = TokenType::I8;
	keywords["i16"] = TokenType::I16;
	keywords["i32"] = TokenType::I32;
	keywords["i64"] = TokenType::I64;
	keywords["u8"] = TokenType::U8;
	keywords["u6"] = TokenType::U16;
	keywords["u32"] = TokenType::U32;
	keywords["u64"] = TokenType::U64;
	keywords["f32"] = TokenType::F32;
	keywords["f64"] = TokenType::F64;
	keywords["char"] = TokenType::Char;
	keywords["bool"] = TokenType::Bool;
	keywords["str"] = TokenType::Str;
	keywords["void"] = TokenType::Void;
}

void Lexer::Lex()
{
	while (cursor < context->input.size())
	{
		SkipWhitespace();

		TokenType type = TokenType::None;
		std::string_view value;
		size_t current_line = line;
		size_t current_column = column;

		switch (Peek())
		{
		case '+':
			type = TokenType::Plus;
			Eat();
			break;
		case '-':
			type = TokenType::Minus;
			Eat();
			break;
		case '*':
			if (Peek(1) == '*')
			{
				type = TokenType::Star;
				Eat();
				Eat();
			}
			else if (Peek(1) == '=')
			{
				type = TokenType::StarEqual;
				Eat();
				Eat();
			}
			else
			{
				type = TokenType::Star;
				Eat();
			}
			break;
		case '%':
			if (Peek(1) == '=')
			{
				type = TokenType::ModuloEqual;
				Eat();
				Eat();
			}
			else
			{
				type = TokenType::Modulo;
				Eat();
			}
			break;
		case '\\':
			type = TokenType::BackSlash;
			Eat();
			break;
		case '/':
			if (Peek(1) == '/')
			{
				SkipToNextLine();
			}
			else if (Peek(1) == '=')
			{
				type = TokenType::SlashEqual;
				Eat();
				Eat();
			}
			else
			{
				type = TokenType::Slash;
				Eat();
			}
			break;
		case '(':
			type = TokenType::LeftParen;
			Eat();
			break;
		case ')':
			type = TokenType::RightParen;
			Eat();
			break;
		case '=':
			if (Peek(1) == '=')
			{
				type = TokenType::EqualEqual;
				Eat();
				Eat();
			}
			else
			{
				type = TokenType::Equal;
				Eat();
			}
			break;
		case '!':
			if (Peek(1) == '=')
			{
				type = TokenType::NotEqual;
				Eat();
				Eat();
			}
			else
			{
				type = TokenType::Not;
				Eat();
			}
			break;

		case '>':
			if (Peek(1) == '=')
			{
				type = TokenType::GreaterThanEqual;
				Eat();
				Eat();
			}
			else
			{
				type = TokenType::GreaterThan;
				Eat();
			}
			break;

		case '<':
			if (Peek(1) == '=')
			{
				type = TokenType::LessThanEqual;
				Eat();
				Eat();
			}
			else
			{
				type = TokenType::LessThan;
				Eat();
			}
			break;

		case '&':
			if (Peek(1) == '&')
			{
				type = TokenType::And;
				Eat();
				Eat();
			}
			else
			{
				type = TokenType::BitAnd;
				Eat();
			}
			break;

		case '|':
			if (Peek(1) == '|')
			{
				type = TokenType::Or;
				Eat();
				Eat();
			}
			else
			{
				type = TokenType::BitOr;
				Eat();
			}
			break;

		case '~':
			type = TokenType::BitNot;
			Eat();
			break;

		case '^':
			type = TokenType::BitXor;
			Eat();
			break;

		case ',':
			type = TokenType::Comma;
			Eat();
			break;
		case '.':
			type = TokenType::Dot;
			Eat();
			break;

		case ';':
			type = TokenType::SemiColon;
			Eat();
			break;

		case '"':
		{
			Eat();
			type = TokenType::StringLiteral;
			auto _start = context->input.data() + cursor;
			size_t start = column;
			size_t end = 0;
			while (Peek() != '"')
			{
				if (Peek() == '\n')
				{
					line++;
					end = column;
					column = 0;
					Eat();
					break;
				}
				Eat();
			}
			if (end == 0)
				end = column;

			value = std::string_view(_start, (end - start));
			Eat();
			break;
		}

		case ':':
			type = TokenType::Colon;
			Eat();
			break;

		case '{':
			type = TokenType::LeftBrace;
			Eat();
			break;

		case '}':
			type = TokenType::RightBrace;
			Eat();
			break;

		case '\0':
			type = TokenType::EndOfFile;
			break;

		default:

			if (std::isalpha(Peek()))
			{

				auto _start = context->input.data() + cursor;
				auto start = column;
				while (std::isalnum(Peek()))
				{
					Eat();
				}
				auto end = column;
				value = std::string_view(_start, (end - start));

				auto is_found = keywords.find(value);
				int length = 0;
				if (is_found != keywords.end())
				{
					type = is_found->second;

					if (type == TokenType::Cpp)
					{
						Eat();
						int blocks = 1;
						auto cpp_start = context->input.data() + cursor;
						auto cpp_start_column = column;

						while (blocks != 0)
						{
							if(Peek() == '{')
							{
								blocks++;
							}
							else if (Peek() == '}')
							{
								blocks--;
								if (blocks == 1)
								{
									blocks = 0;
								}
							}

							Eat();
							length++;
						}

						value = std::string_view(cpp_start, length);
						Eat();
					}
				}
				else
				{
					type = TokenType::Identifier;
				}

			}
			else if (std::isdigit(Peek()))
			{
				value = LexNumber();
				type = TokenType::Number;
			}
			else if (std::isspace(Peek()))
			{
				Eat();
			}
			else {
				// unknown token
				context->Error("Unknown token", current_line, current_column);
				Eat();
			}
		}

		if (type != TokenType::None)
		{
			Token token(type, value, current_line, current_column);
			context->tokens.push_back(token);
		}
	}


	Token token(TokenType::EndOfFile, "", 0, 0);
	context->tokens.push_back(token);
}


void Lexer::SkipWhitespace()
{
	while (std::isspace(Peek()))
	{
		Eat();
	}
}

void Lexer::SkipToNextLine()
{
	while (Peek() != '\n' && cursor < context->input.size())
	{
		Eat();
	}
	if (Peek() == '\n')
	{
		Eat();
	}
}

char Lexer::Peek(int offset)
{
	if (cursor + offset >= context->input.size())
	{
		return '\0';
	}

	return context->input[cursor + offset];
}

char Lexer::Eat()
{
	if (Peek() == '\n')
	{
		line++;
		column = 0;
	}
	else
	{
		column++;
	}
	return context->input[cursor++];
}

std::string_view Lexer::LexNumber()
{
	auto _start = context->input.data() + cursor;
	auto start = column;
	int points_count = 0;

	while (IsNumber(Peek()))
	{
		if (Eat() == '.')
		{
			points_count++;

			if (points_count > 1)
			{
				// TODO: error
				break;
			}
		}
	}

	auto end = column;
	
	return std::string_view(_start, (end - start));
}

bool Lexer::IsNumber(char c)
{
	switch (c)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '_':
	case '.':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'x':
	case 'X':
	case 'o':
	case 'O':
		return true;
	}
	return false;
}
