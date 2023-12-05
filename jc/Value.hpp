#pragma once
#include <iostream>

enum class DataType
{
	None,
	Void,
	Bool,
	String,
	Int8,
	Int16,
	Int32,
	Int64,
	Float,
	Double,
	Uint8,
	Uint16,
	Uint32,
	Uint64,
};

struct Value
{
	DataType type;

	bool IsNumeric()
	{
		switch (type)
		{
		case DataType::Void:
		case DataType::Bool:
		case DataType::String:
			return false;
		case DataType::Int8:
		case DataType::Int16:
		case DataType::Int32:
		case DataType::Int64:
		case DataType::Float:
		case DataType::Double:
		case DataType::Uint8:
		case DataType::Uint16:
		case DataType::Uint32:
		case DataType::Uint64:
			return true;
			break;
		}

		__assume(false);
	}

	union
	{
		long long int integer;
		float float_;
		double double_;
		unsigned long long int unsigned_integer;
	};

	// stream output
	friend std::ostream& operator<<(std::ostream& out, const Value& value)
	{
		switch (value.type)
		{
		case DataType::Void:
		{
			out << value.integer;
			break;
		}
		case DataType::Bool:
		{
			out << value.integer;
			break;
		}
		case DataType::String:
		{
			out << value.integer;
			break;
		}
		case DataType::Int8:
		case DataType::Int16:
		case DataType::Int32:
		case DataType::Int64:
			out << value.integer;
			break;
		case DataType::Uint8:
		case DataType::Uint16:
		case DataType::Uint32:
		case DataType::Uint64:
			out << value.unsigned_integer;
			break;
		case DataType::Float:
		{
			out << value.float_ << "f";
			break;
		}
		case DataType::Double:
		{
			out << value.double_;
			break;
		}
		}
		return out;
	}
};
