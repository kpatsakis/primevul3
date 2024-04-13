bool V8DOMWrapper::maybeDOMWrapper(v8::Handle<v8::Value> value)
{
    if (value.IsEmpty() || !value->IsObject())
        return false;

    v8::Handle<v8::Object> object = v8::Handle<v8::Object>::Cast(value);
    if (!object->InternalFieldCount())
        return false;

    ASSERT(object->InternalFieldCount() >= V8Custom::kDefaultWrapperInternalFieldCount);

    v8::Handle<v8::Value> type = object->GetInternalField(V8Custom::kDOMWrapperTypeIndex);
    ASSERT(type->IsInt32());
    ASSERT(V8ClassIndex::INVALID_CLASS_INDEX < type->Int32Value() && type->Int32Value() < V8ClassIndex::CLASSINDEX_END);

    v8::Handle<v8::Value> wrapper = object->GetInternalField(V8Custom::kDOMWrapperObjectIndex);
    ASSERT(wrapper->IsNumber() || wrapper->IsExternal());

    return true;
}
