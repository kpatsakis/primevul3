v8::Handle<v8::Value> V8DOMWindow::eventAttrGetterCustom(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    v8::Handle<v8::Object> holder = info.This()->FindInstanceInPrototypeChain(V8DOMWindow::GetTemplate(info.GetIsolate(), worldTypeInMainThread(info.GetIsolate())));
    if (holder.IsEmpty())
        return v8::Undefined();

    Frame* frame = V8DOMWindow::toNative(holder)->frame();
    if (!BindingSecurity::shouldAllowAccessToFrame(frame))
        return v8::Undefined();

    ASSERT(frame);
    v8::Local<v8::Context> context = frame->script()->currentWorldContext();
    if (context.IsEmpty())
        return v8::Undefined();

    v8::Handle<v8::String> eventSymbol = V8HiddenPropertyName::event();
    v8::Handle<v8::Value> jsEvent = context->Global()->GetHiddenValue(eventSymbol);
    if (jsEvent.IsEmpty())
        return v8::Undefined();
    return jsEvent;
}
