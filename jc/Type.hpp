#pragma once
#include <string_view>


enum TypeID 
{
	TYPE_UNKNOWN = 0,

	TYPE_INT8,
	TYPE_INT16,
	TYPE_INT32,
	TYPE_INT64,
	TYPE_UINT8,
	TYPE_UINT16,
	TYPE_UINT32,
	TYPE_UINT64,

	TYPE_FLOAT,
	TYPE_DOUBLE,
	
	TYPE_BOOL,
	
	TYPE_STRING,
	TYPE_CHAR,
	
	// NOTE: this should always be last
	TYPE_VOID
};

struct Type
{
	TypeID id;
	std::string_view name;

	Type() 
		: id(TYPE_UNKNOWN)
	{}
	explicit Type(TypeID id, const std::string_view& name);

	bool IsNumeric();
	bool IsIntegral();
	bool IsFloatingPoint();
	bool IsBool();
	bool IsString();
	bool IsVoid();
	bool IsPrimitive();

	bool IsPrimitiveCompatible(const Type& other);

	static Type get_int8_t() { return Type{ TYPE_INT8, "int8_t" }; }
	static Type get_int16_t() { return Type{ TYPE_INT16, "int16_t" }; }
	static Type get_int32_t() { return Type{ TYPE_INT32, "int32_t" }; }
	static Type get_int64_t() { return Type{ TYPE_INT64, "int64_t" }; }
	static Type get_uint8_t() { return Type{ TYPE_UINT8, "uint8_t" }; }
	static Type get_uint16_t() { return Type{ TYPE_UINT16, "uint16_t" }; }
	static Type get_uint32_t() { return Type{ TYPE_UINT32, "uint32_t" }; }
	static Type get_uint64_t() { return Type{ TYPE_UINT64, "uint64_t" }; }
	static Type get_float() { return Type{ TYPE_FLOAT, "float" }; }
	static Type get_double() { return Type{ TYPE_DOUBLE, "double" }; }
	static Type get_string() { return Type{ TYPE_STRING, "std::string" }; }
	static Type get_char() { return Type{ TYPE_CHAR, "char" }; }
	static Type get_bool() { return Type{ TYPE_BOOL, "bool" }; }
	static Type get_void() { return Type{ TYPE_VOID, "void" }; }

	bool operator==(const Type& other) const
	{
		return id == other.id;
	}
};