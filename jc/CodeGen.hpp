#pragma once
#include "Context.hpp"

class CodeGen
{
public:
	explicit CodeGen(Context* context);

	void Generate(std::ostream& out);

private:
	Context* context;
};