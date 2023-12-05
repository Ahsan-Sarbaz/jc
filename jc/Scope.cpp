#include "Scope.hpp"
#include "Context.hpp"

Scope::Scope(size_t index, size_t parent, Context* context)
	: parent(parent)
	, index(index)
	, context(context)
{
}

Variable* Scope::GetVariable(const std::string_view & name)
{
	auto it = variables.find(name);
	if (it != variables.end())
	{
		return it->second;
	}
	if (parent != ~0ull)
	{
		auto p = context->GetScope(parent);
		return p->GetVariable(name);
	}
	return nullptr;
}
