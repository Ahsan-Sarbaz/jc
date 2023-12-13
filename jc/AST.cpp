#include "AST.hpp"
#include <cassert>

Type TypeFromString(const std::string_view& name)
{
	if (name == "i8") {
		return Type::get_uint8_t();
	}
	if (name == "i16") {
		return Type::get_int16_t();
	}
	if (name == "i32") {
		return Type::get_int32_t();
	}
	if (name == "i64") {
		return Type::get_int64_t();
	}
	if (name == "u8") {
		return Type::get_uint8_t();
	}
	if (name == "u16") {
		return Type::get_uint16_t();
	}
	if (name == "u32") {
		return Type::get_uint32_t();
	}
	if (name == "u64") {
		return Type::get_uint64_t();
	}
	if (name == "f32") {
		return Type::get_float();
	}
	if (name == "f64") {
		return Type::get_double();
	}
	if (name == "bool") {
		return Type::get_bool();
	}
	if (name == "str") {
		return Type::get_string();
	}
	if (name == "char") {
		return Type::get_char();
	}
	if (name == "void") {
		return Type::get_void();
	}

	return Type{TYPE_UNKNOWN, "UnknownType"};
}

std::string_view TypeToString(Type data_type)
{
	if (data_type.IsPrimitive())
	{
		switch (data_type.id)
		{
		case TYPE_INT8:
			return "i8";
		case TYPE_INT16:
			return "i16";
		case TYPE_INT32:
			return "i32";
		case TYPE_INT64:
			return "i64";
		case TYPE_UINT8:
			return "u8";
		case TYPE_UINT16:
			return "u16";
		case TYPE_UINT32:
			return "u32";
		case TYPE_UINT64:
			return "u64";
		case TYPE_FLOAT:
			return "f32";
		case TYPE_DOUBLE:
			return "f64";
		case TYPE_BOOL:
			return "bool";
		case TYPE_STRING:
			return "str";
		case TYPE_CHAR:
			return "char";
		case TYPE_VOID:
			return "void";
		}
	}
	else
	{
		return data_type.name;
	}
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

Parameter::Parameter(const std::string_view& name, Type data_type)
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

Variable::Variable(const std::string_view& name, Type data_type)
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

FunctionPrototype::FunctionPrototype(Type return_type, const std::string_view& name, const std::vector<Parameter>& params)
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

DeclarationStatement::DeclarationStatement(std::string_view name, Type data_type, Expression* expression)
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

ExternVariableStatement::ExternVariableStatement(std::string_view name, Type data_type)
	: name(name), data_type(data_type)
{
	this->node_type = ASTNodeType::ExternVariableStatement;
}

ExternFunctionStatement::ExternFunctionStatement(std::string_view name, FunctionPrototype* prototype)
	: name(name), prototype(prototype)
{
	this->node_type = ASTNodeType::ExternFunctionStatement;
}

StructField::StructField(std::string_view name, Type data_type)
	: name(name), data_type(data_type)
{
}

StructDefination::StructDefination(std::string_view name, const std::vector<StructField>& fields)
	: name(name), fields(fields)
{
}

StructDefinationStatement::StructDefinationStatement(StructDefination* defination)
	: defination(defination)
{
	this->node_type = ASTNodeType::StructDefinationStatement;
}

MemberAccessExpression::MemberAccessExpression(Expression* lhs, std::string_view member)
	: lhs(lhs), member(member)
{
	this->node_type = ASTNodeType::MemberAccessExpression;
}
