#include <iostream>

#include "Context.hpp"
#include "Utils.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "CodeGen.hpp"
#include "TypeChecker.hpp"
#include "TypeGenerator.hpp"
#include <cassert>

int main()
{
	auto context = new Context("test.jin");
	context->print_timing = true;
	context->Compile();
	context->PrintMessages();
	system("pause");
	return 0;
}
