FunctionTemplateFactory V8ClassIndex::GetFactory(V8WrapperType type)
{
    switch (type) {
#define MAKE_CASE(type, name)\
    case V8ClassIndex::type: return V8##name::GetTemplate;
    WRAPPER_TYPES(MAKE_CASE)
#undef MAKE_CASE
    default: return NULL;
    }
}
