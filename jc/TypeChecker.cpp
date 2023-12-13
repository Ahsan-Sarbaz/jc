#include "TypeChecker.hpp"
#include <cassert>

TypeChecker::TypeChecker(Context* context)
	: context(context)
{
}

void TypeChecker::Check()
{
	CheckDataType();
}

void TypeChecker::CheckDataType(Expression* expression)
{
	switch (expression->node_type)
	{
	case ASTNodeType::NumberLiteral:
	case ASTNodeType::StringLiteral:
	case ASTNodeType::Variable:
	case ASTNodeType::MemberAccessExpression:
		break;

	case ASTNodeType::CallExpression:
	{
		auto call = static_cast<CallExpression*>(expression);
		auto function = context->GetFunctionPrototype(call->name);
		int i = 0;
		for (auto& arg : call->args)
		{
			CheckDataType(arg);
			if (function->params[i].data_type != arg->data_type)
			{
				if (IsTypeCompatible(arg->data_type, function->params[i].data_type))
				{
					context->Warning("Implicit Conversion: " + std::string(TypeToString(arg->data_type)) + " -> " + 
						std::string(TypeToString(function->params[i].data_type)), arg->line, arg->column);
				}
				else
				{
					context->Error("Type Mismatch: " + std::string(TypeToString(arg->data_type)) + " != " +
						std::string(TypeToString(function->params[i].data_type)), arg->line, arg->column);
				}
			}
			i++;
		}
	}
	break;

	case ASTNodeType::BinaryExpression:
	{
		auto be = static_cast<BinaryExpression*>(expression);
		CheckDataType(be->lhs);
		CheckDataType(be->rhs);
		if (be->lhs->data_type != be->rhs->data_type)
		{
			if (IsTypeCompatible(be->lhs->data_type, be->rhs->data_type))
			{
				context->Warning("Implicit Conversion: " + std::string(TypeToString(be->lhs->data_type)) + " -> " +
					std::string(TypeToString(be->rhs->data_type)), be->lhs->line, be->lhs->column);
			}
			else
			{
				context->Error("Type Mismatch: " + std::string(TypeToString(be->lhs->data_type)) + " != " +
					std::string(TypeToString(be->rhs->data_type)), be->lhs->line, be->lhs->column);
			}
		}
	}
	break;

	case ASTNodeType::AssignmentExpression:
	{
		auto ae = static_cast<AssignmentExpression*>(expression);
		CheckDataType(ae->lhs);
		CheckDataType(ae->rhs);
		if (ae->lhs->data_type != ae->rhs->data_type)
		{
			if (IsTypeCompatible(ae->rhs->data_type, ae->lhs->data_type))
			{
				context->Warning("Implicit Conversion: " + std::string(TypeToString(ae->rhs->data_type)) + " -> " +
					std::string(TypeToString(ae->lhs->data_type)), ae->lhs->line, ae->lhs->column);
			}
			else
			{
				context->Error("Type Mismatch: " + std::string(TypeToString(ae->rhs->data_type)) + " != " +
					std::string(TypeToString(ae->lhs->data_type)), ae->lhs->line, ae->lhs->column);
			}
		}
	}
	break;

	case ASTNodeType::Argument:
	{
		CheckDataType(static_cast<Argument*>(expression)->expression);
		break;
	}

	default:
		assert(false);
		break;
	}
}

void TypeChecker::CheckDataType(Statement* statement)
{
	switch (statement->node_type)
	{
	case ASTNodeType::CallStatement:
	{
		auto call = static_cast<CallStatement*>(statement);
		auto function = context->GetFunctionPrototype(call->name);
		int i = 0;
		for (auto& arg : call->args)
		{
			CheckDataType(arg);
			if (function->params[i].data_type != arg->data_type)
			{
				if (IsTypeCompatible(arg->data_type, function->params[i].data_type))
				{
					context->Warning("Implicit Conversion: " + std::string(TypeToString(arg->data_type)) + " -> " +
						std::string(TypeToString(function->params[i].data_type)), arg->line, arg->column);
				}
				else
				{
					context->Error("Type Mismatch: " + std::string(TypeToString(arg->data_type)) + " != " +
						std::string(TypeToString(function->params[i].data_type)), arg->line, arg->column);
				}
			}
			i++;
		}
	}
	break;
	case ASTNodeType::ExpressionStatement:
		CheckDataType(static_cast<ExpressionStatement*>(statement)->expression);
		break;
	case ASTNodeType::ReturnStatement:
		CheckDataType(static_cast<ReturnStatement*>(statement)->expression);
		break;
	case ASTNodeType::DeclarationStatement:
	{
		auto ds = static_cast<DeclarationStatement*>(statement);
		if (ds->expression)
		{
			CheckDataType(ds->expression);
			if (ds->data_type != ds->expression->data_type)
			{
				if (IsTypeCompatible(ds->data_type, ds->expression->data_type))
				{
					context->Warning("Implicit Conversion: " + std::string(TypeToString(ds->data_type)) + " -> " +
						std::string(TypeToString(ds->expression->data_type)), ds->line, ds->column);
				}
				else
				{
					context->Error("Type Mismatch: " + std::string(TypeToString(ds->data_type)) + " != " +
						std::string(TypeToString(ds->expression->data_type)), ds->line, ds->column);
				}
			}
		}
	}
	break;
	case ASTNodeType::AssignmentStatement:
	{
		auto as = static_cast<AssignmentStatement*>(statement);
		CheckDataType(as->lhs);
		CheckDataType(as->rhs);
		if (as->lhs->data_type != as->rhs->data_type)
		{
			if (IsTypeCompatible(as->lhs->data_type, as->rhs->data_type))
			{
				context->Warning("Implicit Conversion: " + std::string(TypeToString(as->lhs->data_type)) + " -> " +
					std::string(TypeToString(as->rhs->data_type)), as->lhs->line, as->lhs->column);
			}
			else
			{
				context->Error("Type Mismatch: " + std::string(TypeToString(as->lhs->data_type)) + " != " +
					std::string(TypeToString(as->rhs->data_type)), as->lhs->line, as->lhs->column);
			}
		}
	}
	break;
	case ASTNodeType::CppBlock:
	case ASTNodeType::StructDefinationStatement:
	case ASTNodeType::ExternFunctionStatement:
	case ASTNodeType::ExternVariableStatement:
		break;
	default:
		assert(false);
		break;
	}
}

void TypeChecker::CheckDataType()
{
	for (auto& function : context->program->functions)
	{
		for (auto& statement : function->body->statements)
		{
			CheckDataType(statement);
		}
	}
}

bool TypeChecker::IsTypeCompatible(Type is, Type wants)
{
	if (is == wants)
	{
		return true;
	}

	if (is.IsPrimitive() && wants.IsPrimitive())
	{
		if (is.IsPrimitiveCompatible(wants))
		{
			return true;
		}
	}
	else
	{
		if (is.IsPrimitive() || wants.IsPrimitive()) return false;

		auto is_id = context->GetType(is.name);
		auto wants_id = context->GetType(wants.name);

		if (is_id == wants_id)
		{
			return true;
		}

		return false;
	}

	return false;
}
