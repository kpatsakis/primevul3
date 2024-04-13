v8::Handle<v8::Value> V8DOMWrapper::convertWindowToV8Object(DOMWindow* window)
{
    if (!window)
        return v8::Null();
    Frame* frame = window->frame();
    if (!frame)
        return v8::Handle<v8::Object>();

    v8::Handle<v8::Context> currentContext = v8::Context::GetCurrent();
    v8::Handle<v8::Object> currentGlobal = currentContext->Global();
    v8::Handle<v8::Object> windowWrapper = V8DOMWrapper::lookupDOMWrapper(V8ClassIndex::DOMWINDOW, currentGlobal);
    if (!windowWrapper.IsEmpty()) {
        if (convertDOMWrapperToNative<DOMWindow>(windowWrapper) == window)
            return currentGlobal;
    }

    v8::Handle<v8::Context> context = getWrapperContext(frame);
    if (context.IsEmpty())
        return v8::Handle<v8::Object>();

    v8::Handle<v8::Object> global = context->Global();
    ASSERT(!global.IsEmpty());
    return global;
}
