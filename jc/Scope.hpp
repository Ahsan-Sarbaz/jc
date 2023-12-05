#pragma once
#include <unordered_map>
#include <string_view>
#include "AST.hpp"

struct Context;

struct Scope
{
	size_t parent;
	size_t index;
	Context* context;
	std::unordered_map<std::string_view, Variable*> variables;

	explicit Scope(size_t index, size_t parent, Context* context);

	Variable* GetVariable(const std::string_view& name);
};
