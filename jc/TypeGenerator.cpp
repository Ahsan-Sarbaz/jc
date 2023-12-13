#include "TypeGenerator.hpp"
#include <cassert>

TypeGenertaor::TypeGenertaor(Context* context)
	: context(context)
{ }

void TypeGenertaor::Generate()
{
	GenerateDataType();
}

void TypeGenertaor::GenerateDataType(Expression* expression)
{
	switch (expression->node_type)
	{
	case ASTNodeType::NumberLiteral:
		expression->data_type = ((NumberLiteral*)expression)->value.type;
		break;
	case ASTNodeType::StringLiteral:
		expression->data_type = Type::get_string();
		break;
	case ASTNodeType::Variable:
		expression->data_type = context->GetVariableDataType(((Variable*)expression)->name);
		break;
	case ASTNodeType::CallExpression:
	{
		auto call = (CallExpression*)expression;
		expression->data_type = context->GetFunctionReturnType(call->name);
		for (auto& arg : call->args)
		{
			GenerateDataType(arg->expression);
			arg->data_type = arg->expression->data_type;
		}
	}
	break;
	case ASTNodeType::BinaryExpression:
	{
		auto bin = static_cast<BinaryExpression*>(expression);
		GenerateDataType(bin->lhs);
		GenerateDataType(bin->rhs);
		//NOTE: is this even correct bro
		bin->data_type = bin->lhs->data_type;
	}
		break;
	case ASTNodeType::AssignmentExpression:
	{
		auto assign = static_cast<AssignmentExpression*>(expression);
		GenerateDataType(assign->lhs);
		GenerateDataType(assign->rhs);
		assign->data_type = assign->lhs->data_type;
	}
		break;
	case ASTNodeType::Argument:
	{
		auto arg = static_cast<Argument*>(expression);
		GenerateDataType(arg->expression);
		arg->data_type = arg->expression->data_type;
	}
	break;
	case ASTNodeType::MemberAccessExpression:
	{
		auto exp = static_cast<MemberAccessExpression*>(expression);
		GenerateDataType(exp->lhs);
	}
		break;
	default:
		assert(false);
		break;
	}
}

void TypeGenertaor::GenerateDataType(Statement* statement)
{
	switch (statement->node_type)
	{
	case ASTNodeType::CallStatement:
		for (auto& arg : static_cast<CallStatement*>(statement)->args)
		{
			GenerateDataType(arg->expression);
			arg->data_type = arg->expression->data_type;
		}
		break;
	case ASTNodeType::ExpressionStatement:
		GenerateDataType(static_cast<ExpressionStatement*>(statement)->expression);
		break;
	case ASTNodeType::ReturnStatement:
		GenerateDataType(static_cast<ReturnStatement*>(statement)->expression);
		break;
	case ASTNodeType::DeclarationStatement:
		if (static_cast<DeclarationStatement*>(statement)->expression)
		{
			GenerateDataType(static_cast<DeclarationStatement*>(statement)->expression);
		}
		break;
	case ASTNodeType::AssignmentStatement:
		GenerateDataType(static_cast<AssignmentStatement*>(statement)->lhs);
		GenerateDataType(static_cast<AssignmentStatement*>(statement)->rhs);
		break;
	case ASTNodeType::IfStatement:
		GenerateDataType(static_cast<IfStatement*>(statement)->condition);
		break;
	case ASTNodeType::ForStatement:
		GenerateDataType(static_cast<ForStatement*>(statement)->intialization);
		GenerateDataType(static_cast<ForStatement*>(statement)->condition);
		GenerateDataType(static_cast<ForStatement*>(statement)->inc);
		break;
	case ASTNodeType::BlockNode:
		for (auto& statement : static_cast<BlockNode*>(statement)->statements)
		{
			GenerateDataType(statement);
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

void TypeGenertaor::GenerateDataType(Function* function)
{
	for (auto& statement : function->body->statements)
	{
		GenerateDataType(statement);
	}
}

void TypeGenertaor::GenerateDataType()
{
	for (auto& function : context->program->functions)
	{
		GenerateDataType(function);
	}
}
