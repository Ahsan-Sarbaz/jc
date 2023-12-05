#pragma once
#include "Type.hpp"
#include <iostream>

struct Value
{
	Type type;

	bool IsNumeric()
	{
		return type.IsNumeric();
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
		switch (value.type.id)
		{
		case TYPE_VOID:
		{
			out << value.integer;
			break;
		}
		case TYPE_BOOL:
		{
			out << value.integer;
			break;
		}
		case TYPE_STRING:
		{
			out << value.integer;
			break;
		}
		case TYPE_INT8:
		case TYPE_INT16:
		case TYPE_INT32:
		case TYPE_INT64:
			out << value.integer;
			break;
		case TYPE_UINT8:
		case TYPE_UINT16:
		case TYPE_UINT32:
		case TYPE_UINT64:
			out << value.unsigned_integer;
			break;
		case TYPE_FLOAT:
		{
			out << value.float_ << "f";
			break;
		}
		case TYPE_DOUBLE:
		{
			out << value.double_;
			break;
		}
		}
		return out;
	}
};
