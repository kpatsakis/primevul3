v8::Handle<v8::Value> toV8(DOMWindow* window, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{

    if (!window)
        return v8NullWithCheck(isolate);
    Frame* frame = window->frame();
    if (!frame)
        return v8Undefined();

    v8::Handle<v8::Context> currentContext = v8::Context::GetCurrent();
    v8::Handle<v8::Object> currentGlobal = currentContext->Global();
    v8::Handle<v8::Object> windowWrapper = currentGlobal->FindInstanceInPrototypeChain(V8DOMWindow::GetTemplate(isolate, worldTypeInMainThread(isolate)));
    if (!windowWrapper.IsEmpty()) {
        if (V8DOMWindow::toNative(windowWrapper) == window)
            return currentGlobal;
    }

    v8::Handle<v8::Context> context = frame->script()->currentWorldContext();
    if (context.IsEmpty())
        return v8Undefined();

    v8::Handle<v8::Object> global = context->Global();
    ASSERT(!global.IsEmpty());
    return global;
}
