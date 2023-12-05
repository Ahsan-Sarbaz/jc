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
					context->Warning("Implicit Conversion: " + std::string(DataTypeToString(arg->data_type)) + " -> " + 
						std::string(DataTypeToString(function->params[i].data_type)), 0, 0);
				}
				else
				{
					context->Error("Type Mismatch: " + std::string(DataTypeToString(arg->data_type)) + " != " +
						std::string(DataTypeToString(function->params[i].data_type)), 0, 0);
				}
			}
			i++;
		}
	}
	break;

	case ASTNodeType::BinaryExpression:
	{
		auto be = static_cast<BinaryExpression*>(expression);
		if (be->lhs->data_type != be->rhs->data_type)
		{
			if (IsTypeCompatible(be->lhs->data_type, be->rhs->data_type))
			{
				context->Warning("Implicit Conversion: " + std::string(DataTypeToString(be->lhs->data_type)) + " -> " +
					std::string(DataTypeToString(be->rhs->data_type)), 0, 0);
			}
			else
			{
				context->Error("Type Mismatch: " + std::string(DataTypeToString(be->lhs->data_type)) + " != " +
					std::string(DataTypeToString(be->rhs->data_type)), 0, 0);
			}
		}
	}
	break;

	case ASTNodeType::AssignmentExpression:
	{
		auto ae = static_cast<AssignmentExpression*>(expression);
		if (ae->lhs->data_type != ae->rhs->data_type)
		{
			if (IsTypeCompatible(ae->lhs->data_type, ae->rhs->data_type))
			{
				context->Warning("Implicit Conversion: " + std::string(DataTypeToString(ae->lhs->data_type)) + " -> " +
					std::string(DataTypeToString(ae->rhs->data_type)), 0, 0);
			}
			else
			{
				context->Error("Type Mismatch: " + std::string(DataTypeToString(ae->lhs->data_type)) + " != " +
					std::string(DataTypeToString(ae->rhs->data_type)), 0, 0);
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
					context->Warning("Implicit Conversion: " + std::string(DataTypeToString(arg->data_type)) + " -> " +
						std::string(DataTypeToString(function->params[i].data_type)), 0, 0);
				}
				else
				{
					context->Error("Type Mismatch: " + std::string(DataTypeToString(arg->data_type)) + " != " +
						std::string(DataTypeToString(function->params[i].data_type)), 0, 0);
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
		CheckDataType(ds->expression);
		if (ds->data_type != ds->expression->data_type)
		{
			if (IsTypeCompatible(ds->data_type, ds->expression->data_type))
			{
				context->Warning("Implicit Conversion: " + std::string(DataTypeToString(ds->data_type)) + " -> " +
					std::string(DataTypeToString(ds->expression->data_type)), 0, 0);
			}
			else
			{
				context->Error("Type Mismatch: " + std::string(DataTypeToString(ds->data_type)) + " != " +
					std::string(DataTypeToString(ds->expression->data_type)), 0, 0);
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
				context->Warning("Implicit Conversion: " + std::string(DataTypeToString(as->lhs->data_type)) + " -> " +
					std::string(DataTypeToString(as->rhs->data_type)), 0, 0);
			}
			else
			{
				context->Error("Type Mismatch: " + std::string(DataTypeToString(as->lhs->data_type)) + " != " +
					std::string(DataTypeToString(as->rhs->data_type)), 0, 0);
			}
		}
	}
	break;
	case ASTNodeType::CppBlock:
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

bool TypeChecker::IsTypeCompatible(DataType is, DataType wants)
{
	switch (wants)
	{
	case DataType::Void:
		return is == DataType::Void;
	case DataType::Int8:
		return 
			is == DataType::Int8 || is == DataType::Uint8;
	case DataType::Int16:
		return 
			is == DataType::Int16 || is == DataType::Uint16 ||
			is == DataType::Int8 || is == DataType::Uint8;
	case DataType::Int32:
		return 
			is == DataType::Int32 || is == DataType::Uint32 || 
			is == DataType::Int16 || is == DataType::Uint16 || 
			is == DataType::Int8 || is == DataType::Uint8;
	
	case DataType::Int64:
		return 
			is == DataType::Int64 || is == DataType::Uint64 ||
			is == DataType::Int32 || is == DataType::Uint32 ||
			is == DataType::Int16 || is == DataType::Uint16 || 
			is == DataType::Int8 || is == DataType::Uint8;

	case DataType::Float:
		return 
			is == DataType::Float ||
			is == DataType::Int64 || is == DataType::Uint64 ||
			is == DataType::Int32 || is == DataType::Uint32 ||
			is == DataType::Int16 || is == DataType::Uint16 ||
			is == DataType::Int8 || is == DataType::Uint8;

	case DataType::Double:
		return
			is == DataType::Double ||
			is == DataType::Float ||
			is == DataType::Int64 || is == DataType::Uint64 ||
			is == DataType::Int32 || is == DataType::Uint32 ||
			is == DataType::Int16 || is == DataType::Uint16 ||
			is == DataType::Int8 || is == DataType::Uint8;

	case DataType::Bool:
		return is == DataType::Bool;
	
	case DataType::String:
		return is == DataType::String;
	
	case DataType::Uint8:
		return is == DataType::Uint8;
	
	case DataType::Uint16:
		return 
			is == DataType::Uint16 ||
			is == DataType::Uint8;
	
	case DataType::Uint32:
		return
			is == DataType::Uint32 || 
			is == DataType::Uint16 ||
			is == DataType::Uint8;
	
	case DataType::Uint64:
		return 
			is == DataType::Uint64 ||
			is == DataType::Uint32 || 
			is == DataType::Uint16 || 
			is == DataType::Uint8;
	}

	return false;
}
