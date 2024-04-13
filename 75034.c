v8::Handle<v8::Value> V8DOMWrapper::convertDOMImplementationToV8Object(DOMImplementation* impl)
{
    v8::Handle<v8::Object> result = instantiateV8Object(V8ClassIndex::DOMIMPLEMENTATION, V8ClassIndex::DOMIMPLEMENTATION, impl);
    if (result.IsEmpty()) {
        return v8::Null();
    }
    return result;
}
