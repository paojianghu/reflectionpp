#pragma once

#include "accessor.hpp"

namespace rpp {

// a compile-time wraper class of AccessorSimple
template <class Nothing = void>
struct AccessorSimpleWrap {
    template <class Name, class Value>
    using Accessor = AccessorSimple<
        Name,
        Value
    >;

    // TODO
    // template <class Name, class T>
    // using TypeAccessor = AccessorSimple<
    //     Name,
    //     HolderType<T>
    // >;

    // compile-time only
    AccessorSimpleWrap() = delete;
};

// a compile-time wraper class of AccessorObject
template <class Members>
struct AccessorObjectWrap {
    using List = Members;

    template <class Name, class Value>
    using Accessor = AccessorObject<Name, Value, Members>;

    // TODO
    // template <class Name, class T>
    // using TypeAccessor = AccessorObject<
    //     Name, HolderType<T>,
    //     typename AccessorObjectHelper<
    //         HolderLocal<T> /* TODO */, Members
    //     >::MetaList
    // >;


    // compile-time only
    AccessorObjectWrap() = delete;
};

// enable accessorInfer function in the current namespace
#define RPP_ACCESSOR_INFER_INIT() \
    /* an abstract function to infer the default accessor of a type */ \
    rpp::AccessorSimpleWrap<> accessorInfer(...);

RPP_ACCESSOR_INFER_INIT()

// set the default accessor to AccessorObject and bind members (from arguments)
#define RPP_ACCESSOR_BIND(Type, ...) \
    rpp::AccessorObjectWrap< \
        rpp::TypeList<__VA_ARGS__> \
    > accessorInfer(Type &);

// set the default accessor to AccessorObject and bind members (pass a TypeList)
#define RPP_ACCESSOR_BIND_LIST(Type, Members) \
    rpp::AccessorObjectWrap< \
        Members \
    > accessorInfer(Type &);

// get the default accessor
#define RPP_ACCESSOR_GET(Name, Value) \
    decltype( \
        accessorInfer((*static_cast<Value *>(nullptr))()) \
    )::Accessor<Name, Value>

// append an accessor to a TypeList
// wrapper of RPP_HOLDER_TYPE
#define RPP_ACCESSOR_APPEND_TYPE(Object, Type) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Type), RPP_HOLDER_TYPE(Object::Type))>

// append an accessor to a TypeList
// wrapper of RPP_HOLDER_CONST
#define RPP_ACCESSOR_APPEND_CONST(Object, Value) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Value), RPP_HOLDER_CONST(Object::Value))>

// append an accessor to a TypeList
// wrapper of RPP_HOLDER_REF
#define RPP_ACCESSOR_APPEND_REF(Object, Value) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Value), RPP_HOLDER_REF(Object::Value))>

// append an accessor to a TypeList
// wrapper of RPP_HOLDER_MEMBER
#define RPP_ACCESSOR_APPEND_MEMBER(Object, Value) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Value), RPP_HOLDER_MEMBER(Object, Value))>

// append accessors fetched from AccessorObjectWrap to a TypeList
#define RPP_ACCESSOR_APPEND_BASE(Derived, Base) \
    ::AppendList<decltype( \
        accessorInfer(*static_cast<Base *>(nullptr)) \
    )::List>

// build a TypeList of accessors
// some helper macros
#define RPP_ACCESSOR_LIST_ITEM_IMPL(Member, Method, ...) \
    , RPP_ACCESSOR_LIST), RPP_ACCESSOR_APPEND_##Method, Member
#define RPP_ACCESSOR_LIST_ITEM(...) \
    RPP_ACCESSOR_LIST_ITEM_IMPL(__VA_ARGS__, MEMBER, )
#ifndef __
    #define __ RPP_ACCESSOR_LIST_ITEM
#endif
#define RPP_ACCESSOR_LIST_BEGIN(Object, Next) \
    Next##_1(Object
#define RPP_ACCESSOR_LIST_1(Object, Method, Member, Next) \
    Method(Object, Member) Next##_2(Object
#define RPP_ACCESSOR_LIST_2(Object, Method, Member, Next) \
    Method(Object, Member) Next##_1(Object
#define RPP_ACCESSOR_LIST_END_1(...) /* nothing */
#define RPP_ACCESSOR_LIST_END_2(...) /* nothing */
// the main macro
#define RPP_ACCESSOR_BIND_OBJECT(Object, Members) \
    rpp::AccessorObjectWrap< \
        rpp::TypeList<> \
        RPP_ACCESSOR_LIST_BEGIN( \
            Object \
            Members \
            , RPP_ACCESSOR_LIST_END) \
        ) \
    > accessorInfer(Object &);

}
