#pragma once
#include <iostream>
#include <vector>
#include <sstream> 
#include <fstream> 
#include "Value.hpp"

enum class ASTNodeType
{
	Expression,
	Statement,
	Function,
	Program,
	Parameter,
	FunctionPrototype,
	NumberLiteral,
	BinaryExpression,
	Variable,
	CallStatement,
	AssignmentStatement,
	IfStatement,
	CallExpression,
	ForStatement,
	ReturnStatement,
	ContinueStatement,
	BreakStatement,
	ExpressionStatement,
	DeclarationStatement,
	AssignmentExpression,
	UnaryExpression,
	StringLiteral,
	BlockNode,
	Argument,
	CppBlock,
	ExternVariableStatement,
	ExternFunctionStatement,
	StructDefinationStatement,
	MemberAccessExpression,
};

struct Scope;

struct ASTNode
{
	size_t line;
	size_t column;
	ASTNodeType node_type;
};

struct Parameter : public ASTNode
{
	std::string_view name;
	Type data_type;

	explicit Parameter(const std::string_view& name, Type data_type);
};

struct FunctionPrototype : public ASTNode
{
	std::string_view name;
	std::vector<Parameter> params;
	Type return_type;

	explicit FunctionPrototype(Type return_type, const std::string_view& name, const std::vector<Parameter>& params);
};

struct Expression : public ASTNode
{
	Type data_type = Type{ TypeID::TYPE_UNKNOWN , ""};
};

struct Argument : public Expression
{
	Expression* expression;
	
	explicit Argument(Expression* expression);
};

struct NumberLiteral : public Expression
{
	Value value;

	explicit NumberLiteral(Value value);
};

struct StringLiteral : public Expression
{
	std::string_view value;

	explicit StringLiteral(std::string_view value);
};

enum class BinaryOperatorType
{
	Plus,
	Minus,
	Multiply,
	Divide,
	Modulo,
	Equal,
	EqualEqual,
	NotEqual,
	LessThan,
	LessThanEqual,
	GreaterThan,
	GreaterThanEqual,
	And,
	Or,
};

struct BinaryExpression : public Expression
{
	Expression* lhs;
	BinaryOperatorType op;
	Expression* rhs;

	explicit BinaryExpression(Expression* lhs, BinaryOperatorType op, Expression* rhs);
};

enum class UnaryOperatorType
{
	Plus,
	Minus,
	Not,
};

struct UnaryExpression : public Expression
{
	UnaryOperatorType op;
	Expression* expression;

	explicit UnaryExpression(UnaryOperatorType op, Expression* expression);
};

struct Statement : public ASTNode
{
};

struct BlockNode : public Statement
{
	std::vector<Statement*> statements;
	size_t scope_id;

	explicit BlockNode(const std::vector<Statement*>& statements, size_t scope_id);
};

struct ExpressionStatement : public Statement
{
	Expression* expression;

	explicit ExpressionStatement(Expression* expression);
};

struct ReturnStatement : public Statement
{
	Expression* expression;

	explicit ReturnStatement(Expression* expression);
};

struct Function : public ASTNode
{
	FunctionPrototype* prototype;
	BlockNode* body;

	explicit Function(FunctionPrototype* prototype, BlockNode* body);
};

struct CallExpression : public Expression
{
	std::string_view name;
	std::vector<Argument*> args;

	explicit CallExpression(const std::string_view& name, const std::vector<Argument*>& args);
};

struct Variable : public Expression
{
	std::string_view name;

	explicit Variable(const std::string_view& name, Type data_type);
};

struct Program : public ASTNode
{
	std::vector<Function*> functions;
	std::vector<Statement*> statements;

	explicit Program(const std::vector<Function*>& functions, const std::vector<Statement*>& statements);
};

struct AssignmentStatement : public Statement
{
	Expression* lhs;
	Expression* rhs;

	explicit AssignmentStatement(Expression* lhs, Expression* rhs);
};

struct AssignmentExpression : public Expression
{
	Expression* lhs;
	Expression* rhs;

	explicit AssignmentExpression(Expression* lhs, Expression* rhs);
};

struct CallStatement : public Statement
{
	std::string_view name;
	std::vector<Argument*> args;
	
	explicit CallStatement(std::string_view name, std::vector<Argument*> args);
};

struct IfStatement : public Statement
{
	Expression* condition;
	BlockNode* if_body;
	BlockNode* else_body;

	explicit IfStatement(Expression* condition, BlockNode* if_body, BlockNode* else_body = nullptr);
};

struct ForStatement : public Statement
{
	Statement* intialization;
	Expression* condition;
	Expression* inc;
	BlockNode* body;

	explicit ForStatement(Statement* intialization, Expression* condition, Expression* inc, BlockNode* body);
};

struct DeclarationStatement : public Statement
{
	std::string_view name;
	Type data_type;
	Expression* expression;
	
	explicit DeclarationStatement(std::string_view name, Type data_type, Expression* expression);
};

struct CppBlock : public Statement
{
	std::string_view code;

	explicit CppBlock(std::string_view code);
};

struct ExternVariableStatement : public Statement
{
	std::string_view name;
	Type data_type;
	
	explicit ExternVariableStatement(std::string_view name, Type data_type);
};

struct ExternFunctionStatement : public Statement
{
	std::string_view name;
	FunctionPrototype* prototype;

	explicit ExternFunctionStatement(std::string_view name, FunctionPrototype* prototype);
};

struct StructField
{
	std::string_view name;
	Type data_type;

	StructField() = default;

	explicit StructField(std::string_view name, Type data_type);
};

struct StructDefination
{
	std::string_view name;
	std::vector<StructField> fields;

	StructDefination() = default;

	explicit StructDefination(std::string_view name, const std::vector<StructField>& fields);
};

struct StructDefinationStatement : public Statement
{
	StructDefination* defination;

	explicit StructDefinationStatement(StructDefination* defination);
};

struct MemberAccessExpression : public Expression
{
	Expression* lhs;
	std::string_view member;

	explicit MemberAccessExpression(Expression* lhs, std::string_view member);
};

Type TypeFromString(const std::string_view& name);
std::string_view TypeToString(Type data_type);
