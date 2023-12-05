#include "Parser.hpp"
#include <charconv>
#include <cassert>


Parser::Parser(Context* context)
	:context(context)
	, cursor(0)
{

}

void Parser::Parse()
{
	std::vector<Function*> functions;
	std::vector<Statement*> statements;

	while (Peek().type != TokenType::EndOfFile)
	{
		if (Peek().type == TokenType::Function)
		{
			functions.push_back(ParseFunction());
		}
		else if (Peek().type == TokenType::Let)
		{
			statements.push_back(ParseDeclarationStatement());
		}
		else if (Peek().type == TokenType::Cpp)
		{
			statements.push_back(ParseCpp());
		}
		else if (Peek().type == TokenType::Struct)
		{
			statements.push_back(ParseStruct());
		}
		else if (Peek().type == TokenType::Extern)
		{
			if (Peek(1).type == TokenType::Function)
			{
				statements.push_back(ParseExternFunctionStatement());
			}
			else
			{
				statements.push_back(ParseExternVariableStatement());
			}
		}
		else
		{
			Eat();
			context->Error("Unexpected token: " + std::string(Peek().value), Peek().line, Peek().column);
		}
	}

	context->CreateProgram(functions, statements);
}

Expression* Parser::ParseFactor()
{
	// <factor> ::= <number> | <identifier> | <call> | "(" <expression> ")"
	if (Peek().type == TokenType::Number)
	{
		auto number = Eat().value;
		std::string number_without_underscores = std::string(number);

		if (number.find('_'))
		{
			number_without_underscores.erase
			(std::remove(number_without_underscores.begin(), number_without_underscores.end(), '_'), number_without_underscores.end());

			number = number_without_underscores;
		}

		if (number.find('.') != std::string::npos)
		{
			double numeric_value;
			Value value{};
			std::from_chars(number.data(), number.data() + number.size(), numeric_value);
			if (numeric_value >= FLT_MIN && numeric_value <= FLT_MAX)
			{
				value.type = Type::get_float();
				value.float_ = float(numeric_value);
			}
			else
			{
				value.type = Type::get_double();
				value.double_ = double(numeric_value);

			}
			return new NumberLiteral(value);
		}
		else
		{
			int64_t numeric_value;

			int base = 10;
			if (number[0] == '0')
			{
				number = number.substr(1);
				if (number[0] == 'x' || number[0] == 'X')
				{
					base = 16;
					number = number.substr(1);
				}
				else if (number[0] == 'b' || number[0] == 'B')
				{
					base = 2;
					number = number.substr(1);
				}
				else if (number[0] == 'o' || number[0] == 'O')
				{
					base = 8;
					number = number.substr(1);
				}
			}

			std::from_chars(number.data(), number.data() + number.size(), numeric_value, base);

			Value value;

			if (numeric_value > 0)
			{
				if (numeric_value <= UINT8_MAX)
				{
					value.type = Type::get_int8_t();
					value.unsigned_integer = (uint64_t)numeric_value;
				}
				else if (numeric_value <= UINT16_MAX)
				{
					value.type = Type::get_int16_t();
					value.unsigned_integer = (uint64_t)numeric_value;
				}
				else if (numeric_value <= UINT32_MAX)
				{
					value.type = Type::get_int32_t();
					value.unsigned_integer = (uint64_t)numeric_value;
				}
				else
				{
					value.type = Type::get_int64_t();
					value.unsigned_integer = (uint64_t)numeric_value;
				}
			}
			else
			{
				if (numeric_value <= UINT8_MAX)
				{
					value.type = Type::get_uint8_t();
					value.unsigned_integer = (uint64_t)numeric_value;
				}
				else if (numeric_value <= INT16_MAX)
				{
					value.type = Type::get_uint16_t();
					value.unsigned_integer = (uint64_t)numeric_value;
				}
				else if (numeric_value <= INT32_MAX)
				{
					value.type = Type::get_uint32_t();
					value.unsigned_integer = (uint64_t)numeric_value;
				}
				else
				{
					value.type = Type::get_uint64_t();
					value.unsigned_integer = (uint64_t)numeric_value;
				}
			}

			return new NumberLiteral(value);
		}
	}
	else if (Peek().type == TokenType::Identifier)
	{
		if (Peek(1).type == TokenType::LeftParen)
		{
			return ParseCallExpression();
		}

		auto identifier = Eat().value;
		auto v = context->GetVariableFromScope(identifier);
		if (v == nullptr)
		{
			std::string message = "Variable not found: " + std::string(identifier);
			context->Error(message, Peek().line, Peek().column);
		}
		return new Variable(identifier, Type{});
	}
	else if (Peek().type == TokenType::LeftParen)
	{
		Eat();
		auto expression = ParseExpression();
		Expect(TokenType::RightParen);
		return expression;
	}
	else if (Peek().type == TokenType::StringLiteral)
	{
		auto value = Eat().value;
		return new StringLiteral(value);
	}
	else if (IsUnaryOperator(Peek().type))
	{
		auto expression = ParseUnaryExpression();
		return expression;
	}


	return nullptr;
}

Expression* Parser::ParseTerm()
{
	// <term> ::= <factor> | <term> "*" <factor> | <term> "/" <factor> | <term> "%" <factor>

	auto lhs = ParseFactor();
	switch (Peek().type)
	{
		case TokenType::Star:
		case TokenType::Slash:
		case TokenType::Modulo:
		{
			auto op = GetBinaryOperator(Eat().type);
			auto rhs = ParseFactor();
			return new BinaryExpression(lhs, op, rhs);
		}
	}

	return lhs;
}

Expression* Parser::ParseExpression()
{
	//<expression> :: = <term> | <expression> ">" <term> | <expression> "<" <term> | <expression> ">=" <term> | < expression> "<=" <term>
	//<term> :: = <factor> | <term> "+" <factor> | <term> "-" <factor>

	auto lhs = ParseTerm();
	if(Peek().type == TokenType::SemiColon)
	{
		return lhs;
	}
	else if (IsBinaryOperator(Peek().type))
	{
		auto op = GetBinaryOperator(Eat().type);
		auto rhs = ParseExpression();
		return new BinaryExpression(lhs, op, rhs);
	}
	else if (Peek().type == TokenType::Equal)
	{
		Eat();
		auto rhs = ParseExpression();
		return new AssignmentExpression(lhs, rhs);
	}

	return lhs;
}

AssignmentExpression* Parser::ParseAssignmentExpression()
{
	// <assigment> ::= <identifier> "=" <expression>
	auto name = Expect(TokenType::Identifier).value;
	Expect(TokenType::Equal);
	auto expression = ParseExpression();
	return new AssignmentExpression(new Variable(name, Type{}), expression);
}

AssignmentStatement* Parser::ParseAssignmentStatement()
{
	// <assigment> ::= <identifier> "=" <expression> ";"
	auto expression = ParseAssignmentExpression();
	Expect(TokenType::SemiColon);
	return new AssignmentStatement(expression->lhs, expression->rhs);
}

ReturnStatement* Parser::ParseReturnStatement()
{
	// <return_expression> ::= "return "<expression> ";"
	Expect(TokenType::Return);
	auto expression = ParseExpression();
	Expect(TokenType::SemiColon);
	return new ReturnStatement(expression);
}

CallExpression* Parser::ParseCallExpression()
{
	// <call> ::= <identifier> "(" <args> ")"
	// <args> ::= <expression> ("," <expression>)*
	// <expression> ::= <expression> | <call_expression>
	auto name = Expect(TokenType::Identifier).value;
	auto p = context->GetFunctionPrototype(name);
	if (p == nullptr)
	{
		context->Error("Function not found: " + std::string(name), Peek().line, Peek().column);
		return nullptr;
	}

	Expect(TokenType::LeftParen);
	std::vector<Argument*> args;
	while (Peek().type != TokenType::RightParen)
	{
		if (Peek().type == TokenType::Identifier && Peek(1).type == TokenType::LeftParen)
		{
			args.push_back(new Argument(ParseCallExpression()));
		}
		else
		{
			args.push_back(new Argument(ParseExpression()));
		}
		if (Peek().type == TokenType::RightParen)
			break;
		Expect(TokenType::Comma);
	}
	Expect(TokenType::RightParen);
	return new CallExpression(name, args);
}

CallStatement* Parser::ParseCallStatement()
{
	// <call> ::= <identifier> "(" <args> ")" ;
	// <args> ::= <expression> ("," <expression>)*
	auto name = Expect(TokenType::Identifier).value;
	Expect(TokenType::LeftParen);
	std::vector<Argument*> args;
	while (Peek().type != TokenType::RightParen)
	{
		args.push_back(new Argument(ParseExpression()));

		if (Peek().type == TokenType::RightParen)
			break;
		Expect(TokenType::Comma);
	}
	Expect(TokenType::RightParen);
	Expect(TokenType::SemiColon);
	return new CallStatement(name, args);
}

BlockNode* Parser::ParseBlock(bool create_new_scope)
{
	if(create_new_scope)
		context->CreateScope();

	// <block> ::= "{" (<statements>* | e ) | ( <blocks>* | e ) "}"
	Expect(TokenType::LeftBrace);
	std::vector<Statement*> statements;
	while (Peek().type != TokenType::RightBrace)
	{

		if(Peek().type == TokenType::LeftBrace)
		{
			statements.push_back(ParseBlock());
			continue;
		}

		auto statement = ParseStatement();
		if (statement)
		{
			statements.push_back(statement);
		}
		else
		{
			break;
		}
	}

	Expect(TokenType::RightBrace);

	if (create_new_scope)
		context->PopScope();

	return new BlockNode(statements, context->GetCurrentScopeIndex());
}

IfStatement* Parser::ParseIfStatement()
{
	// <if_statement> ::= "if" <expression> "{" <body> "}" ("else" "{" <body> "}" | e)
	// eat if
	Expect(TokenType::If);
	// eat <expression>
	auto expression = ParseExpression();
	Expect(TokenType::LeftBrace);
	// eat <body>
	auto body = ParseBlock();
	Expect(TokenType::RightBrace);
	// eat else if available
	BlockNode* elseBody = nullptr;
	if (Peek().type == TokenType::Else)
	{
		Expect(TokenType::Else);
		// eat <body>
		elseBody = ParseBlock();
	}

	return new IfStatement(expression, body, elseBody);
}

ForStatement* Parser::ParseForStatement()
{
	// <for_statement> ::= "for" "(" <assignment> ";" <expression> ";" <expression> ")" "{" <body> "}"
		// eat for
	Expect(TokenType::For);
	// eat (
	Expect(TokenType::LeftParen);
	// eat <assignment>
	Statement* assignment = nullptr;
	if (Peek().type != TokenType::SemiColon)
	{
		assignment = ParseStatement();
	}
	else
	{
		Expect(TokenType::SemiColon);
	}
	// eat ; (not required)
//	Expect(TokenType::SemiColon);
	// eat <expression>
	Expression* condition = nullptr;
	if (Peek().type != TokenType::SemiColon)
	{
		condition = ParseExpression();
	}
	// eat ;
	Expect(TokenType::SemiColon);
	// eat <expression>
	Expression* inc = nullptr;
	if (Peek().type != TokenType::RightParen)
	{
		inc = ParseExpression();
	}
	// eat )
	Expect(TokenType::RightParen);
	// eat <body>
	auto body = ParseBlock();
	return new ForStatement(assignment, condition, inc, std::move(body));
}

ExpressionStatement* Parser::ParseExpressionStatement()
{
	auto expression = ParseExpression();
	Expect(TokenType::SemiColon);
	return new ExpressionStatement(expression);
}

Statement* Parser::ParseStatement()
{
	if (Peek().type == TokenType::Identifier)
	{
		if (Peek(1).type == TokenType::LeftParen)
		{
			// TODO: does this even make sense?
			return ParseCallStatement();
		}
		else if (Peek(1).type == TokenType::Equal)
		{
			return ParseAssignmentStatement();
		}
	
		// why are we here?
		assert(false);
	}
	else if(Peek().type == TokenType::Cpp)
	{
		return ParseCpp();
	}
	else if (Peek().type == TokenType::Return)
	{
		return ParseReturnStatement();
	}
	else if (Peek().type == TokenType::If)
	{
		return ParseIfStatement();
	}
	else if (Peek().type == TokenType::For)
	{
		return ParseForStatement();
	}
	else if (Peek().type == TokenType::Extern)
	{
		if (Peek(1).type == TokenType::Function)
		{
			return ParseExternFunctionStatement();
		}
		else
		{
			return ParseExternVariableStatement();
		}
	}
	else if (Peek().type == TokenType::Let)
	{
		return ParseDeclarationStatement();
	}
	else
	{
		return ParseExpressionStatement();
	}

	_assume(false);
}

Function* Parser::ParseFunction()
{
	context->PushScope();

	// eat fn
	Expect(TokenType::Function);
	// eat function name
	auto name = Expect(TokenType::Identifier).value;
	// eat (
	Expect(TokenType::LeftParen);

	std::vector<Parameter> params;
	while (Peek().type != TokenType::RightParen)
	{
		auto arg_name = Expect(TokenType::Identifier).value;
		auto datatype = ExpectType();
		params.emplace_back(arg_name, datatype);

		context->AddVariableToScope(arg_name, datatype);

		if (Peek().type == TokenType::RightParen)
			break;
		Expect(TokenType::Comma);
	}
	Expect(TokenType::RightParen);
	auto datatype = ExpectType();
	// don't create a new scope
	auto body = ParseBlock(false);
	FunctionPrototype* protype = new FunctionPrototype(datatype, name, params);
	context->functions[name] = protype;

	context->PopScope();

	return new Function(protype, body);
}

UnaryExpression* Parser::ParseUnaryExpression()
{
	// <unary> ::= <unary_operator> <expression>
	auto op_token = Eat().type;
	auto expression = ParseExpression();

	switch (op_token)
	{
	case TokenType::Plus:
		return new UnaryExpression(UnaryOperatorType::Plus, expression);
		break;
	case TokenType::Minus:
		return new UnaryExpression(UnaryOperatorType::Minus, expression);
		break;
	case TokenType::Not:
		return new UnaryExpression(UnaryOperatorType::Not, expression);
		break;
	default:
		__assume(false);
	}
}

DeclarationStatement* Parser::ParseDeclarationStatement()
{
	// <assigment> ::= "let" <identifier> ( <data_type> | e ) "=" <expression> ";" 
	Expect(TokenType::Let);
	auto name = Expect(TokenType::Identifier).value;
	auto data_type = ExpectType();
	if (Peek().type != TokenType::Equal)
	{
		Expect(TokenType::SemiColon);
		context->AddVariableToScope(name, data_type);
		return new DeclarationStatement(name, data_type, nullptr);
	}
	else
	{
		Expect(TokenType::Equal);
	}
	auto expression = ParseExpression();
	Expect(TokenType::SemiColon);

	context->AddVariableToScope(name, data_type);

	return new DeclarationStatement(name, data_type, expression);
}

CppBlock* Parser::ParseCpp()
{
	return new CppBlock(Expect(TokenType::Cpp).value);
}

ExternFunctionStatement* Parser::ParseExternFunctionStatement()
{
	// <extern_func> := "extern" "fn" <identifier> "(" <args> ")" <return_type> ";"
	
	Expect(TokenType::Extern);
	Expect(TokenType::Function);
	auto name = Expect(TokenType::Identifier).value;
	Expect(TokenType::LeftParen);
	std::vector<Parameter> params;
	while (Peek().type != TokenType::RightParen)
	{
		auto arg_name = Expect(TokenType::Identifier).value;
		auto datatype = ExpectType();
		params.emplace_back(arg_name, datatype);
		if (Peek().type == TokenType::RightParen)
			break;
		Expect(TokenType::Comma);
	}
	Expect(TokenType::RightParen);
	auto return_type = ExpectType();
	Expect(TokenType::SemiColon);

	auto prototype = new FunctionPrototype(return_type, name, params);
	context->functions[name] = prototype;

	return new ExternFunctionStatement(name, prototype);
}

ExternVariableStatement* Parser::ParseExternVariableStatement()
{
	// <extern_var> := "extern" <identifier> <data_type> ";"
	Expect(TokenType::Extern);
	auto name = Expect(TokenType::Identifier).value;
	auto data_type = ExpectType();
	Expect(TokenType::SemiColon);

	context->AddVariableToScope(name, data_type);

	return new ExternVariableStatement(name, data_type);
}

StructDefinationStatement* Parser::ParseStruct()
{
	Expect(TokenType::Struct);
	auto name = Expect(TokenType::Identifier).value;
	Expect(TokenType::LeftBrace);
	std::vector<StructField> fields;
	while (Peek().type != TokenType::RightBrace)
	{
		auto field_name = Expect(TokenType::Identifier).value;
		auto field_type = ExpectType();
		fields.emplace_back(field_name, field_type);
		if (Peek().type == TokenType::RightBrace)
			break;
		Expect(TokenType::Comma);
	}
	Expect(TokenType::RightBrace);

	context->CreateType(name);

	return new StructDefinationStatement(name, fields);
}

Token Parser::Expect(TokenType type)
{
	if (Peek().type == type)
	{
		return Eat();
	}
	else
	{
		// TODO: fix this
		auto error = "Expected something, got something else";
		context->Error(error, Peek().line, Peek().column);
		assert(false);
	}

	__assume(false);
}

Type Parser::ExpectType()
{
	auto token = Eat();
	switch (token.type)
	{
	case TokenType::I8: return Type::get_int8_t();
	case TokenType::I16: return Type::get_int16_t();
	case TokenType::I32: return Type::get_int32_t();
	case TokenType::I64: return Type::get_int64_t();
	case TokenType::U8: return Type::get_uint8_t();
	case TokenType::U16: return Type::get_uint16_t();
	case TokenType::U32: return Type::get_uint32_t();
	case TokenType::U64: return Type::get_uint64_t();
	case TokenType::F32: return Type::get_float();
	case TokenType::F64: return Type::get_double();
	case TokenType::Bool: return Type::get_bool();
	case TokenType::Str: return Type::get_string();
	case TokenType::Char: return Type::get_char();
	case TokenType::Void: return Type::get_void();
	default: return Type(TYPE_UNKNOWN, token.value);
	}

	__assume(false);
}

bool Parser::IsKeyword(TokenType type)
{
	switch (type)
	{
	case TokenType::I8: return true;
	case TokenType::I16: return true;
	case TokenType::I32: return true;
	case TokenType::I64: return true;
	case TokenType::U8: return true;
	case TokenType::U16: return true;
	case TokenType::U32: return true;
	case TokenType::U64: return true;
	case TokenType::F32: return true;
	case TokenType::F64: return true;
	case TokenType::Bool: return true;
	case TokenType::Str: return true;
	case TokenType::Char: return true;
	case TokenType::Void: return true;
	case TokenType::If: return true;
	case TokenType::Else: return true;
	case TokenType::For: return true;
	case TokenType::Let: return true;
	case TokenType::Return: return true;
	case TokenType::Function: return true;
	case TokenType::Extern: return true;
	case TokenType::Cpp: return true;
	case TokenType::Struct: return true;
	}

	return false;
}

bool Parser::IsDataType(TokenType type)
{

	switch (type)
	{
	case TokenType::I8: return true;
	case TokenType::I16: return true;
	case TokenType::I32: return true;
	case TokenType::I64: return true;
	case TokenType::U8: return true;
	case TokenType::U16: return true;
	case TokenType::U32: return true;
	case TokenType::U64: return true;
	case TokenType::F32: return true;
	case TokenType::F64: return true;
	case TokenType::Bool: return true;
	case TokenType::Str: return true;
	case TokenType::Char: return true;
	case TokenType::Void: return true;
	}

	return false;
}

bool Parser::IsUnaryOperator(TokenType type)
{
	switch (type)
	{
	case TokenType::Plus: return true;
	case TokenType::Minus: return true;
	case TokenType::Not: return true;
	}

	return false;
}

bool Parser::IsBinaryOperator(TokenType type)
{
	switch (type)
	{
	case TokenType::Plus: return true;
	case TokenType::Minus: return true;
	case TokenType::Star: return true;
	case TokenType::Slash: return true;
	case TokenType::Modulo: return true;
	case TokenType::And: return true;
	case TokenType::Or: return true;
	case TokenType::EqualEqual: return true;
	case TokenType::NotEqual: return true;
	case TokenType::LessThan: return true;
	case TokenType::LessThanEqual: return true;
	case TokenType::GreaterThan: return true;
	case TokenType::GreaterThanEqual: return true;
	}
	return false;
}

BinaryOperatorType Parser::GetBinaryOperator(TokenType type)
{
	switch (type)
	{
	case TokenType::Plus: return BinaryOperatorType::Plus;
	case TokenType::Minus: return BinaryOperatorType::Minus;
	case TokenType::Star: return BinaryOperatorType::Multiply;
	case TokenType::Slash: return BinaryOperatorType::Divide;
	case TokenType::Modulo: return BinaryOperatorType::Modulo;
	case TokenType::And: return BinaryOperatorType::And;
	case TokenType::Or: return BinaryOperatorType::Or;
	case TokenType::Equal: return BinaryOperatorType::Equal;
	case TokenType::NotEqual: return BinaryOperatorType::NotEqual;
	case TokenType::LessThan: return BinaryOperatorType::LessThan;
	case TokenType::LessThanEqual: return BinaryOperatorType::LessThanEqual;
	case TokenType::GreaterThan: return BinaryOperatorType::GreaterThan;
	case TokenType::GreaterThanEqual: return BinaryOperatorType::GreaterThanEqual;
	}
	
	assert(false);
	__assume(false);
}
