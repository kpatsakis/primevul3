v8::Handle<v8::Value> V8DOMWindow::toStringMethodCustom(const v8::Arguments& args)
{
    v8::Handle<v8::Object> domWrapper = args.This()->FindInstanceInPrototypeChain(V8DOMWindow::GetTemplate(args.GetIsolate(), worldTypeInMainThread(args.GetIsolate())));
    if (domWrapper.IsEmpty())
        return args.This()->ObjectProtoToString();
    return domWrapper->ObjectProtoToString();
}
