void V8DOMWrapper::setJSWrapperForDOMObject(void* object, v8::Persistent<v8::Object> wrapper)
{
    ASSERT(V8DOMWrapper::maybeDOMWrapper(wrapper));
#ifndef NDEBUG
    V8ClassIndex::V8WrapperType type = V8DOMWrapper::domWrapperType(wrapper);
    switch (type) {
#define MAKE_CASE(TYPE, NAME) case V8ClassIndex::TYPE:
        ACTIVE_DOM_OBJECT_TYPES(MAKE_CASE)
        ASSERT_NOT_REACHED();
#undef MAKE_CASE
    default:
        break;
    }
#endif
    getDOMObjectMap().set(object, wrapper);
}
