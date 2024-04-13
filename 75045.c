v8::Local<v8::Function> V8DOMWrapper::getConstructor(V8ClassIndex::V8WrapperType type, v8::Handle<v8::Value> objectPrototype)
{
    v8::Handle<v8::FunctionTemplate> functionTemplate = getTemplate(type);
    v8::TryCatch tryCatch;
    v8::Local<v8::Function> value = functionTemplate->GetFunction();
    if (value.IsEmpty())
        return v8::Local<v8::Function>();
    value->Set(v8::String::New("__proto__"), objectPrototype);
    return value;
}
