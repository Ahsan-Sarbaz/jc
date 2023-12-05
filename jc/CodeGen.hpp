#pragma once
#include "Context.hpp"

class CodeGen
{
public:
	explicit CodeGen(Context* context);

	void Generate(std::ostream& out);

private:
	void Output(std::ostream& out, Program* program);
	void Output(std::ostream& out, Statement* statement);
	void Output(std::ostream& out, Function* function);
	void Output(std::ostream& out, Expression* expression);
	void Output(std::ostream& out, FunctionPrototype* prototype);
	void Output(std::ostream& out, CallExpression* call);
	void Output(std::ostream& out, Variable* variable);
	void Output(std::ostream& out, NumberLiteral* number);
	void Output(std::ostream& out, StringLiteral* string_literal);
	void Output(std::ostream& out, BinaryExpression* expression);
	void Output(std::ostream& out, ExpressionStatement* statement);
	void Output(std::ostream& out, ReturnStatement* statement);
	void Output(std::ostream& out, CallStatement* statement);
	void Output(std::ostream& out, ForStatement* for_statement);
	void Output(std::ostream& out, AssignmentStatement* assigment);
	void Output(std::ostream& out, AssignmentExpression* expression);
	void Output(std::ostream& out, DeclarationStatement* declaration);
	void Output(std::ostream& out, BlockNode* block);
	void Output(std::ostream& out, IfStatement* if_statement);
	void Output(std::ostream& out, CppBlock* cpp_block);
	void Output(std::ostream& out, StructDefinationStatement* struct_defination);

	Context* context;
};