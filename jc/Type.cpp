#include "Type.hpp"

Type::Type(TypeID id, const std::string_view& name)
	: id(id)
	, name(name)
{

}

bool Type::IsNumeric()
{
	return IsIntegral() || IsFloatingPoint();
}

bool Type::IsIntegral()
{
	return id >= TYPE_INT8 && id <= TYPE_UINT64;
}

bool Type::IsFloatingPoint()
{
	return id >= TYPE_FLOAT && id <= TYPE_DOUBLE;
}

bool Type::IsBool()
{
	return id == TYPE_BOOL;
}

bool Type::IsString()
{
	return id == TYPE_STRING;
}

bool Type::IsVoid()
{
	return id == TYPE_VOID;
}

bool Type::IsPrimitive()
{
	return id > TYPE_UNKNOWN && id <= TYPE_VOID;
}

bool Type::IsPrimitiveCompatible(const Type& other)
{
	if (*this == other) return true;

	switch (other.id)
	{
	case TypeID::TYPE_VOID:
		return id == TypeID::TYPE_VOID;
	case TypeID::TYPE_INT8:
		return
			id == TypeID::TYPE_INT8 || id == TypeID::TYPE_UINT8;
	case TypeID::TYPE_INT16:
		return
			id == TypeID::TYPE_INT16 || id == TypeID::TYPE_UINT16 ||
			id == TypeID::TYPE_INT8 || id == TypeID::TYPE_UINT8;
	case TypeID::TYPE_INT32:
		return
			id == TypeID::TYPE_INT32 || id == TypeID::TYPE_UINT32 ||
			id == TypeID::TYPE_INT16 || id == TypeID::TYPE_UINT16 ||
			id == TypeID::TYPE_INT8 || id == TypeID::TYPE_UINT8;

	case TypeID::TYPE_INT64:
		return
			id == TypeID::TYPE_INT64 || id == TypeID::TYPE_UINT64 ||
			id == TypeID::TYPE_INT32 || id == TypeID::TYPE_UINT32 ||
			id == TypeID::TYPE_INT16 || id == TypeID::TYPE_UINT16 ||
			id == TypeID::TYPE_INT8 || id == TypeID::TYPE_UINT8;

	case TypeID::TYPE_FLOAT:
		return
			id == TypeID::TYPE_FLOAT ||
			id == TypeID::TYPE_INT64 || id == TypeID::TYPE_UINT64 ||
			id == TypeID::TYPE_INT32 || id == TypeID::TYPE_UINT32 ||
			id == TypeID::TYPE_INT16 || id == TypeID::TYPE_UINT16 ||
			id == TypeID::TYPE_INT8 || id == TypeID::TYPE_UINT8;

	case TypeID::TYPE_DOUBLE:
		return
			id == TypeID::TYPE_DOUBLE ||
			id == TypeID::TYPE_FLOAT ||
			id == TypeID::TYPE_INT64 || id == TypeID::TYPE_UINT64 ||
			id == TypeID::TYPE_INT32 || id == TypeID::TYPE_UINT32 ||
			id == TypeID::TYPE_INT16 || id == TypeID::TYPE_UINT16 ||
			id == TypeID::TYPE_INT8 || id == TypeID::TYPE_UINT8;

	case TypeID::TYPE_BOOL:
		return id == TypeID::TYPE_BOOL;

	case TypeID::TYPE_STRING:
		return id == TypeID::TYPE_STRING;

	case TypeID::TYPE_UINT8:
		return id == TypeID::TYPE_UINT8;

	case TypeID::TYPE_UINT16:
		return
			id == TypeID::TYPE_UINT16 ||
			id == TypeID::TYPE_UINT8;

	case TypeID::TYPE_UINT32:
		return
			id == TypeID::TYPE_UINT32 ||
			id == TypeID::TYPE_UINT16 ||
			id == TypeID::TYPE_UINT8;

	case TypeID::TYPE_UINT64:
		return
			id == TypeID::TYPE_UINT64 ||
			id == TypeID::TYPE_UINT32 ||
			id == TypeID::TYPE_UINT16 ||
			id == TypeID::TYPE_UINT8;
	}

	return false;
}
