#pragma once
#include "Lexer.hpp"
#include "AST.hpp"
#include "Context.hpp"


class Parser
{
public:
	Parser(Context*);

	void Parse();

	inline Token Peek(int offset = 0) { return context->tokens[cursor + offset]; }
	inline Token Eat() { return context->tokens[cursor++]; }
	Token Expect(TokenType type);
	Type ExpectType();
	bool IsKeyword(TokenType type);
	bool IsDataType(TokenType type);
	bool IsUnaryOperator(TokenType type);
	bool IsBinaryOperator(TokenType type);
	BinaryOperatorType GetBinaryOperator(TokenType type);

private:
	Expression* ParseFactor();
	Expression* ParseTerm();
	Expression* ParseExpression();
	AssignmentExpression* ParseAssignmentExpression();
	AssignmentStatement* ParseAssignmentStatement();
	ExpressionStatement* ParseExpressionStatement();
	ReturnStatement* ParseReturnStatement();
	CallExpression* ParseCallExpression();
	CallStatement* ParseCallStatement();
	BlockNode* ParseBlock(bool create_new_scope = true);
	IfStatement* ParseIfStatement();
	ForStatement* ParseForStatement();
	Statement* ParseStatement();
	Function* ParseFunction();
	UnaryExpression* ParseUnaryExpression();
	DeclarationStatement* ParseDeclarationStatement();
	CppBlock* ParseCpp();
	ExternFunctionStatement* ParseExternFunctionStatement();
	ExternVariableStatement* ParseExternVariableStatement();
	StructDefinationStatement* ParseStruct();

	Context* context;
	int cursor = 0;
};
