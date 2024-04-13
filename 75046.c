v8::Local<v8::Function> V8DOMWrapper::getConstructor(V8ClassIndex::V8WrapperType type, DOMWindow* window)
{
    Frame* frame = window->frame();
    if (!frame)
        return v8::Local<v8::Function>();

    v8::Handle<v8::Context> context = getWrapperContext(frame);
    if (context.IsEmpty())
        return v8::Local<v8::Function>();
    v8::Context::Scope scope(context);

    return getConstructor(type, V8Proxy::getHiddenObjectPrototype(context));
}
