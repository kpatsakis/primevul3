void* V8DOMWrapper::convertToSVGPODTypeImpl(V8ClassIndex::V8WrapperType type, v8::Handle<v8::Value> object)
{
    return isWrapperOfType(object, type) ? convertDOMWrapperToNative<void>(v8::Handle<v8::Object>::Cast(object)) : 0;
}
