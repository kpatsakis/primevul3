void V8DOMWrapper::setDOMWrapper(v8::Handle<v8::Object> object, int type, void* cptr)
{
    ASSERT(object->InternalFieldCount() >= 2);
    object->SetPointerInInternalField(V8Custom::kDOMWrapperObjectIndex, cptr);
    object->SetInternalField(V8Custom::kDOMWrapperTypeIndex, v8::Integer::New(type));
}
