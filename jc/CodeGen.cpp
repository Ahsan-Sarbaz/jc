#include "CodeGen.hpp"
#include <cassert>

// outstream for std::string_view

const char* DataTypeToCPPTypeString(DataType type)
{
	switch (type)
	{
	case DataType::Int8: return "int8_t";
	case DataType::Int16: return "int16_t";
	case DataType::Int32: return "int32_t";
	case DataType::Int64: return "int64_t";
	case DataType::Uint8: return "uint8_t";
	case DataType::Uint16: return "uint16_t";
	case DataType::Uint32: return "uint32_t";
	case DataType::Uint64: return "uint64_t";
	case DataType::Float: return "float";
	case DataType::Double: return "double";
	case DataType::Bool: return "bool";
	case DataType::String: return "std::string";
	case DataType::Void: return "void";
	}

	return "";
}

const char* BinaryOperatorToCPPString(BinaryOperatorType op)
{
	switch (op)
	{
	case BinaryOperatorType::Plus: return "+";
	case BinaryOperatorType::Minus: return "-";
	case BinaryOperatorType::Multiply: return "*";
	case BinaryOperatorType::Divide: return "/";
	case BinaryOperatorType::Modulo: return "%";
	case BinaryOperatorType::Equal: return "==";
	case BinaryOperatorType::NotEqual: return "!=";
	case BinaryOperatorType::LessThan: return "<";
	case BinaryOperatorType::LessThanEqual: return "<=";
	case BinaryOperatorType::GreaterThan: return ">";
	case BinaryOperatorType::GreaterThanEqual: return ">=";
	case BinaryOperatorType::And: return "&&";
	case BinaryOperatorType::Or: return "||";
	}

	return "";
}

void Output(std::ostream& out, FunctionPrototype* prototype);
void Output(std::ostream& out, CallExpression* call);
void Output(std::ostream& out, Variable* variable);
void Output(std::ostream& out, NumberLiteral* number);
void Output(std::ostream& out, StringLiteral* string_literal);
void Output(std::ostream& out, Expression* expression);
void Output(std::ostream& out, ExpressionStatement* statement);
void Output(std::ostream& out, ReturnStatement* statement);
void Output(std::ostream& out, CallStatement* statement);
void Output(std::ostream& out, Statement* statement);
void Output(std::ostream& out, Program* program);
void Output(std::ostream& out, Function* function);
void Output(std::ostream& out, ForStatement* for_statement);
void Output(std::ostream& out, AssignmentStatement* assigment);
void Output(std::ostream& out, AssignmentExpression* expression);
void Output(std::ostream& out, DeclarationStatement* declaration);
void Output(std::ostream& out, BlockNode* block);
void Output(std::ostream& out, IfStatement* if_statement);
void Output(std::ostream& out, CppBlock* cpp_block);

CodeGen::CodeGen(Context* context)
	: context(context)
{
}

void CodeGen::Generate(std::ostream& out)
{
	Output(out, context->program);
}

void Output(std::ostream& out, IfStatement* if_statement)
{
	out << "if (";
	Output(out, if_statement->condition);
	out << ")";
	Output(out, if_statement->if_body);
	if (if_statement->else_body)
	{
		out << "else";
		Output(out, if_statement->else_body);
	}
}

void Output(std::ostream& out, CppBlock* cpp_block)
{
	out << cpp_block->code;
}

void Output(std::ostream& out, BlockNode* block)
{
	out << "\n{\n";
	for (auto statement : block->statements)
	{
		out << "\t";
		Output(out, statement);
		out << "\n";
	}
	out << "}";
}

void Output(std::ostream& out, StringLiteral* string_literal)
{
	out << "\"" << string_literal->value << "\"";
}

void Output(std::ostream& out, DeclarationStatement* declaration)
{
	out << DataTypeToCPPTypeString(declaration->data_type) << " " << declaration->name << " = ";
	Output(out, declaration->expression);
	out << ";\n";
}

void Output(std::ostream& out, AssignmentStatement* assigment)
{
	Output(out, assigment->lhs);
	out << " = ";
	Output(out, assigment->rhs);
}

void Output(std::ostream& out, AssignmentExpression* expression)
{
	Output(out, expression->lhs);
	out << " = ";
	Output(out, expression->rhs);
}


void Output(std::ostream& out, ForStatement* for_statement)
{
	out << "for (";
	if (for_statement->intialization)
		Output(out, for_statement->intialization);
	out << "; ";
	if(for_statement->condition)
		Output(out, for_statement->condition);
	out << "; ";
	if (for_statement->inc)
		Output(out, for_statement->inc);
	out << ")";
	
	Output(out, for_statement->body);

}


void Output(std::ostream& out, FunctionPrototype* prototype)
{
	out << DataTypeToCPPTypeString(prototype->return_type) << " " << prototype->name << "(";
	for (int i = 0; i < prototype->params.size(); ++i)
	{
		auto& argument = prototype->params[i];
		out << DataTypeToCPPTypeString(argument.data_type) << " " << argument.name;
		if (i < prototype->params.size() - 1)
		{
			out << ", ";
		}
	}
	out << ")";
}

void Output(std::ostream& out, CallExpression* call)
{
	out << call->name << "(";
	for (int i = 0; i < call->args.size(); ++i)
	{
		auto argument_expression = call->args[i];
		Output(out, argument_expression);
		if (i < call->args.size() - 1)
		{
			out << ", ";
		}
	}
	out << ")";
}

void Output(std::ostream& out, Variable* variable)
{
	out << variable->name;
}

void Output(std::ostream& out, NumberLiteral* number)
{
	out << number->value;
}

void Output(std::ostream& out, BinaryExpression* expression)
{
	out << "(";
	Output(out, expression->lhs);
	out << " "<< BinaryOperatorToCPPString(expression->op) << " ";
	Output(out, expression->rhs);
	out << ")";
}

void Output(std::ostream& out, Expression* expression)
{
	switch (expression->node_type)
	{
	case ASTNodeType::CallExpression: Output(out, static_cast<CallExpression*>(expression)); break;
	case ASTNodeType::BinaryExpression: Output(out, static_cast<BinaryExpression*>(expression)); break;
	case ASTNodeType::Variable: Output(out, static_cast<Variable*>(expression)); break;
	case ASTNodeType::NumberLiteral: Output(out, static_cast<NumberLiteral*>(expression)); break;
	case ASTNodeType::StringLiteral: Output(out, static_cast<StringLiteral*>(expression)); break;
	case ASTNodeType::AssignmentExpression: Output(out, static_cast<AssignmentExpression*>(expression)); break;
	case ASTNodeType::Argument: Output(out, static_cast<Argument*>(expression)->expression); break;
	default:
		assert(false);
		break;
	}
}

void Output(std::ostream& out, ExpressionStatement* statement)
{
	Output(out, statement->expression);
	out << ";\n";
}

void Output(std::ostream& out, ReturnStatement* statement)
{
	out << "return ";
	Output(out, statement->expression);
	out << ";\n";
}

void Output(std::ostream& out, CallStatement* statement)
{
	out << statement->name << "(";
	for (int i = 0; i < statement->args.size(); ++i)
	{
		auto argument_expression = statement->args[i];
		Output(out, argument_expression);
		if (i < statement->args.size() - 1)
		{
			out << ", ";
		}
	}
	out << ");\n";
}

void Output(std::ostream& out, Statement* statement)
{
	switch (statement->node_type)
	{
	case ASTNodeType::ExpressionStatement: Output(out, static_cast<ExpressionStatement*>(statement)); break;
	case ASTNodeType::ReturnStatement: Output(out, static_cast<ReturnStatement*>(statement)); break;
	case ASTNodeType::CallStatement: Output(out, static_cast<CallStatement*>(statement)); break;
	case ASTNodeType::AssignmentStatement: Output(out, static_cast<AssignmentStatement*>(statement)); break;
	case ASTNodeType::ForStatement: Output(out, static_cast<ForStatement*>(statement)); break;
	case ASTNodeType::IfStatement: Output(out, static_cast<IfStatement*>(statement)); break;
	case ASTNodeType::DeclarationStatement: Output(out, static_cast<DeclarationStatement*>(statement)); break;
	case ASTNodeType::BlockNode: Output(out, static_cast<BlockNode*>(statement)); break;
	case ASTNodeType::CppBlock: Output(out, static_cast<CppBlock*>(statement)); break;

	case ASTNodeType::ExternFunctionStatement:
	case ASTNodeType::ExternVariableStatement:
		// no output
		break;
	default: assert(false); break;
	}
}

void Output(std::ostream& out, Function* function)
{
	Output(out, function->prototype);
	Output(out, function->body);
}

void Output(std::ostream& out, Program* program)
{
	
	for (auto statement : program->statements)
	{
		Output(out, statement);
	}

	for (auto function : program->functions)
	{
		Output(out, function);
		out << "\n";
	}
}