#pragma once
#include "AST.hpp"
#include "Parser.hpp"

class TypeChecker
{
public:
	explicit TypeChecker(Context* context);

	void Check();

private:
	void CheckDataType(Expression* expression);
	void CheckDataType(Statement* statement);
	void CheckDataType();

	bool IsTypeCompatible(Type is, Type wants);

	Context* context;
};
