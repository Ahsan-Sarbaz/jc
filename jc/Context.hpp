#pragma once
#include <unordered_map>
#include <string_view>
#include "Token.hpp"
#include "AST.hpp"

struct Scope;

struct Message
{
	std::string text;
	size_t line;
	size_t column;
};

struct Context
{
	std::string_view input;

	std::unordered_map<std::string_view, FunctionPrototype*> functions;
	std::unordered_map<std::string_view, Variable*> variables;
	std::vector<Scope*> scopes;
	Scope* current_scope;
	Scope* root_scope;
	std::vector<Message> errors;
	std::vector<Message> warnings;
	std::vector<Token> tokens;
	Program* program;
	bool print_timing = false;
	bool print_warings = false;

	Context(const char* file_path);

	void Compile();

	void CreateProgram(const std::vector<Function*>&, const std::vector<Statement*>&);

	Variable* GetVariableFromScope(const std::string_view& name);
	Variable* AddVariableToScope(const std::string_view& name, DataType data_type);
	DataType GetVariableDataType(const std::string_view& name);
	Scope* GetParentScope();
	Scope* CreateScope();
	Scope* GetScope(size_t index);
	size_t GetCurrentScopeIndex();
	FunctionPrototype* GetFunctionPrototype(const std::string_view& name);
	DataType GetFunctionReturnType(const std::string_view& name);

	void PushScope();
	void PopScope();

	void Error(const std::string& text, size_t line, size_t column);
	void Warning(const std::string& text, size_t line, size_t column);

	void PrintMessages();

};
