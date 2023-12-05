#pragma once
#include "AST.hpp"
#include "Parser.hpp"

class TypeGenertaor
{
public:
	explicit TypeGenertaor(Context* context);

	void Generate();

private:
	void GenerateDataType(Expression* expression);
	void GenerateDataType(Statement* statement);
	void GenerateDataType(Function* function);
	void GenerateDataType();

	Context* context;
};
