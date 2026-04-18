#include "script_system/type_info.h"

using namespace scripting;

TypeInfo TypeInfo::getInt() {
    static TypeInfo typeInfo(TypeKind::INT);
    return typeInfo;
}

TypeInfo TypeInfo::getFloat() {
    static TypeInfo typeInfo(TypeKind::FLOAT);
    return typeInfo;
}

TypeInfo TypeInfo::getString() {
    static TypeInfo typeInfo(TypeKind::STRING);
    return typeInfo;
}
