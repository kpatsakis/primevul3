v8::Handle<v8::Value> V8DOMWrapper::convertSVGElementInstanceToV8Object(SVGElementInstance* instance)
{
    if (!instance)
        return v8::Null();

    v8::Handle<v8::Object> existingInstance = getDOMSVGElementInstanceMap().get(instance);
    if (!existingInstance.IsEmpty())
        return existingInstance;

    instance->ref();

    v8::Handle<v8::Object> result = instantiateV8Object(V8ClassIndex::SVGELEMENTINSTANCE, V8ClassIndex::SVGELEMENTINSTANCE, instance);
    if (!result.IsEmpty()) {
        getDOMSVGElementInstanceMap().set(instance, v8::Persistent<v8::Object>::New(result));
    }
    return result;
}
