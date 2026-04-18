#pragma once

#include "script_system/type_kind.h"

namespace scripting {

class Class;

class TypeInfo {
public:
    TypeInfo() = default;

    TypeInfo(TypeKind kind) : typeKind(kind) {}

    TypeInfo(const Class* cls) : typeKind(TypeKind::OBJECT), objectClass(cls) {}

    static TypeInfo getInt();
    static TypeInfo getFloat();
    static TypeInfo getString();

    TypeKind getKind() const {
        return typeKind;
    }

    const Class* getClass() const {
        return objectClass;
    }

private:
    TypeKind typeKind = TypeKind::INVALID;
    const Class* objectClass = nullptr;
};

} // namespace scripting