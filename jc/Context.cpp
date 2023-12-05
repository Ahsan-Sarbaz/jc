#include "Context.hpp"
#include "Scope.hpp"
#include "Utils.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "TypeGenerator.hpp"
#include "TypeChecker.hpp"
#include "CodeGen.hpp"
#include <cassert>

Context::Context(const char* file_path)
	:program(nullptr)
{
	input = read_file_into_string_view(file_path);
	current_scope = new Scope(0, -1, this);
	root_scope = current_scope;
	scopes.push_back(current_scope);
}

void Context::Compile()
{
	auto compile_start = get_time();

	{
		Lexer lexer(this);
		auto lexer_start = get_time();
		lexer.Lex();
		auto lexer_time = get_time_diff_ms(lexer_start);
		if (print_timing)
			std::cout << "Lexer Took: " << lexer_time << "ms" << std::endl;
	}

	if(errors.empty())
	{
		Parser parser(this);
		auto parser_start = get_time();
		parser.Parse();
		auto parser_time = get_time_diff_ms(parser_start);
		if (print_timing)
			std::cout << "Parser Took: " << parser_time << "ms" << std::endl;
	}
	else
	{
		std::cout << "Not Parsing because of previous errors" << std::endl;
	}

	if (errors.empty())
	{
		TypeGenertaor generator(this);
		auto typegen_start = get_time();
		generator.Generate();
		auto typegen_time = get_time_diff_ms(typegen_start);
		if (print_timing)
			std::cout << "TypeGenertaor Took: " << typegen_time << "ms" << std::endl;
	}
	else
	{
		std::cout << "Not Type Generating because of previous errors" << std::endl;
	}

	if (errors.empty())
	{
		TypeChecker typechecker(this);
		auto typechecker_start = get_time();
		typechecker.Check();
		auto typechecker_time = get_time_diff_ms(typechecker_start);
		if (print_timing)
			std::cout << "TypeChecker Took: " << typechecker_time << "ms" << std::endl;
	}
	else
	{
		std::cout << "Not Type Checking because of previous errors" << std::endl;
	}

	if (errors.empty())
	{
		CodeGen codegen(this);
		auto codegen_start = get_time();
		codegen.Generate(std::cout);
		auto codegen_time = get_time_diff_ms(codegen_start);
		if (print_timing)
			std::cout << "CodeGen Took: " << codegen_time << "ms" << std::endl;
	}
	else
	{
		std::cout << "Not Code Generating because of previous errors" << std::endl;
	}

	auto compile_time = get_time_diff_ms(compile_start);
	if (print_timing)
		std::cout << "Compile Total Time: " << compile_time << "ms" << std::endl;
}

void Context::CreateProgram(const std::vector<Function*>& functions, const std::vector<Statement*>& statements)
{
	program = new Program(functions, statements);
}

Variable* Context::GetVariableFromScope(const std::string_view& name)
{
	return current_scope->GetVariable(name);
}

Variable* Context::AddVariableToScope(const std::string_view& name, DataType data_type)
{
	current_scope->variables[name] = new Variable(name, data_type);
	variables[name] = current_scope->variables[name];
	return current_scope->variables[name];
}

DataType Context::GetVariableDataType(const std::string_view& name)
{
	auto it = variables.find(name);
	if (it != variables.end())
	{
		return it->second->data_type;
	}

	return DataType::None;
}

Scope* Context::GetParentScope()
{
	if (current_scope->parent == -1)
	{
		return root_scope;
	}

	return scopes[current_scope->parent];
}

Scope* Context::CreateScope()
{
	auto scope = new Scope(scopes.size(), current_scope->index, this);
	scopes.push_back(scope);
	current_scope = scope;
	return scope;
}

Scope* Context::GetScope(size_t index)
{
	return scopes[index];
}

size_t Context::GetCurrentScopeIndex()
{
	return scopes.size() - 1;
}

FunctionPrototype* Context::GetFunctionPrototype(const std::string_view& name)
{
	auto it = functions.find(name);
	if (it != functions.end())
	{
		return it->second;
	}

	return nullptr;
}

DataType Context::GetFunctionReturnType(const std::string_view& name)
{
	auto it = functions.find(name);
	if (it != functions.end())
	{
		return it->second->return_type;
	}

	assert(false);
	__assume(false);
}

void Context::PushScope()
{
	current_scope = CreateScope();
}

void Context::PopScope()
{
	current_scope = GetParentScope();
}

void Context::Error(const std::string& text, size_t line, size_t column)
{
	Message message;
	message.text = text;
	message.line = line;
	message.column = column;
	errors.push_back(message);
}

void Context::Warning(const std::string& text, size_t line, size_t column)
{
	if (!print_warings)
		return;

	Message message;
	message.text = text;
	message.line = line;
	message.column = column;
	warnings.push_back(message);
}

void Context::PrintMessages()
{
	for (auto& message : errors)
	{
		if (message.line == 0)
		{
			std::cout << "Error: " << message.text << std::endl;
		}
		else
		{
			std::cout << "Error: " << message.text << " at line " << message.line << ", column " << message.column << std::endl;
		}
	}

	
	if (print_warings) for (auto& message : warnings)
	{
		if (message.line == 0)
		{
			std::cout << "Warning: " << message.text << std::endl;
		}
		else
		{
			std::cout << "Warning: " << message.text << " at line " << message.line << ", column " << message.column << std::endl;
		}
	}
}
