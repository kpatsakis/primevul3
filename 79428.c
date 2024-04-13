void V8DOMWindow::eventAttrSetterCustom(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    v8::Handle<v8::Object> holder = info.This()->FindInstanceInPrototypeChain(V8DOMWindow::GetTemplate(info.GetIsolate(), worldTypeInMainThread(info.GetIsolate())));
    if (holder.IsEmpty())
        return;

    Frame* frame = V8DOMWindow::toNative(holder)->frame();
    if (!BindingSecurity::shouldAllowAccessToFrame(frame))
        return;

    ASSERT(frame);
    v8::Local<v8::Context> context = frame->script()->currentWorldContext();
    if (context.IsEmpty())
        return;

    v8::Handle<v8::String> eventSymbol = V8HiddenPropertyName::event();
    context->Global()->SetHiddenValue(eventSymbol, value);
}
