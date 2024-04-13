v8::Persistent<v8::FunctionTemplate>* V8ClassIndex::GetCache(V8WrapperType type)
{
    switch (type) {
#define MAKE_CASE(type, name)\
    case V8ClassIndex::type: return &name##_cache_;
    ALL_WRAPPER_TYPES(MAKE_CASE)
#undef MAKE_CASE
    default:
        ASSERT(false);
        return NULL;
  }
}
