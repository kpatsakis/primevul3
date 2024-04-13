V8ClassIndex::V8WrapperType V8DOMWrapper::domWrapperType(v8::Handle<v8::Object> object)
{
    ASSERT(V8DOMWrapper::maybeDOMWrapper(object));
    v8::Handle<v8::Value> type = object->GetInternalField(V8Custom::kDOMWrapperTypeIndex);
    return V8ClassIndex::FromInt(type->Int32Value());
}
