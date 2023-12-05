#include "AST.hpp"
#include <cassert>

DataType DataTypeFromString(const std::string_view& name)
{
	if (name == "i8") {
		return DataType::Int8;
	}
	if (name == "i16") {
		return DataType::Int16;
	}
	if (name == "i32") {
		return DataType::Int32;
	}
	if (name == "i64") {
		return DataType::Int64;
	}
	if (name == "u8") {
		return DataType::Uint8;
	}
	if (name == "u16") {
		return DataType::Uint16;
	}
	if (name == "u32") {
		return DataType::Uint32;
	}
	if (name == "u64") {
		return DataType::Uint64;
	}
	if (name == "f32") {
		return DataType::Float;
	}
	if (name == "f64") {
		return DataType::Double;
	}
	if (name == "bool") {
		return DataType::Bool;
	}
	if (name == "str") {
		return DataType::String;
	}
	if (name == "void") {
		return DataType::Void;
	}

	return DataType::Void;
}

const char* DataTypeToString(DataType data_type)
{
	switch (data_type)
	{
	case DataType::None: return "none";
	case DataType::Int8: return "i8";
	case DataType::Int16: return "i16";
	case DataType::Int32: return "i32";
	case DataType::Int64: return "i64";
	case DataType::Uint8: return "u8";
	case DataType::Uint16: return "u16";
	case DataType::Uint32: return "u32";
	case DataType::Uint64: return "u64";
	case DataType::Float: return "f32";
	case DataType::Double: return "f64";
	case DataType::Bool: return "bool";
	case DataType::String: return "str";
	case DataType::Void: return "void";
	}

	assert(false);
	__assume(false);
}

NumberLiteral::NumberLiteral(Value value)
	:value(value)
{
	this->node_type = ASTNodeType::NumberLiteral;
}

BinaryExpression::BinaryExpression(Expression* lhs, BinaryOperatorType op, Expression* rhs)
	:lhs(lhs)
	, op(op)
	, rhs(rhs)
{
	this->node_type = ASTNodeType::BinaryExpression;
}

ReturnStatement::ReturnStatement(Expression* expression)
	:expression(expression)
{
	this->node_type = ASTNodeType::ReturnStatement;
}

Parameter::Parameter(const std::string_view& name, DataType data_type)
	:name(name)
	, data_type(data_type)
{
	this->node_type = ASTNodeType::Parameter;
}

Function::Function(FunctionPrototype* prototype, BlockNode* body)
	:prototype(prototype)
	, body(std::move(body))
{
	this->node_type = ASTNodeType::Function;
}

Variable::Variable(const std::string_view& name, DataType data_type)
	:name(name)
{
	this->node_type = ASTNodeType::Variable;
	this->data_type = data_type;
}

Program::Program(const std::vector<Function*>& functions, const std::vector<Statement*>& statements)
	:functions(functions)
	, statements(statements)
{
	this->node_type = ASTNodeType::Program;
}

FunctionPrototype::FunctionPrototype(DataType return_type, const std::string_view& name, const std::vector<Parameter>& params)
	:return_type(return_type)
	, name(name)
	, params(params)
{
	this->node_type = ASTNodeType::FunctionPrototype;
}

CallExpression::CallExpression(const std::string_view& name, const std::vector<Argument*>& args)
	:name(name)
	, args(args)
{
	this->node_type = ASTNodeType::CallExpression;
}

AssignmentStatement::AssignmentStatement(Expression* lhs, Expression* rhs)
	: lhs(lhs), rhs(rhs)
{
	this->node_type = ASTNodeType::AssignmentStatement;
}

CallStatement::CallStatement(std::string_view name, std::vector<Argument*> args)
	: name(name), args(args)
{
	this->node_type = ASTNodeType::CallStatement;
}

IfStatement::IfStatement(Expression* condition, BlockNode* if_body, BlockNode* else_body)
	: condition(condition)
	, if_body(std::move(if_body))
	, else_body(std::move(else_body))
{
	this->node_type = ASTNodeType::IfStatement;
}

ForStatement::ForStatement(Statement* intialization, Expression* condition, Expression* inc, BlockNode* body)
	: intialization(intialization)
	, condition(condition)
	, inc(inc)
	, body(std::move(body))
{
	this->node_type = ASTNodeType::ForStatement;
}

ExpressionStatement::ExpressionStatement(Expression* expression)
	: expression(expression)
{
	this->node_type = ASTNodeType::ExpressionStatement;
}

AssignmentExpression::AssignmentExpression(Expression* lhs, Expression* rhs)
	: lhs(lhs) , rhs(rhs)
{
	this->node_type = ASTNodeType::AssignmentExpression;
}

UnaryExpression::UnaryExpression(UnaryOperatorType op, Expression* expression)
	: op(op) , expression(expression)
{
	this->node_type = ASTNodeType::UnaryExpression;
}

DeclarationStatement::DeclarationStatement(std::string_view name, DataType data_type, Expression* expression)
	: name(name), data_type(data_type), expression(expression)
{
	this->node_type = ASTNodeType::DeclarationStatement;
}

StringLiteral::StringLiteral(std::string_view value)
{
	this->value = value;
	this->node_type = ASTNodeType::StringLiteral;
}

BlockNode::BlockNode(const std::vector<Statement*>& statements, size_t scope_id)
	: statements(statements)
	, scope_id(scope_id)
{
	this->node_type = ASTNodeType::BlockNode;
}

Argument::Argument(Expression* expression)
	: expression(expression)
{
	this->node_type = ASTNodeType::Argument;
}

CppBlock::CppBlock(std::string_view code)
	: code(code)
{
	this->node_type = ASTNodeType::CppBlock;
}

ExternVariableStatement::ExternVariableStatement(std::string_view name, DataType data_type)
	: name(name), data_type(data_type)
{
	this->node_type = ASTNodeType::ExternVariableStatement;
}

ExternFunctionStatement::ExternFunctionStatement(std::string_view name, FunctionPrototype* prototype)
	: name(name), prototype(prototype)
{
	this->node_type = ASTNodeType::ExternFunctionStatement;
}
