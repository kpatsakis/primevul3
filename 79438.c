v8::Handle<v8::Value> toV8ForMainWorld(DOMWindow* window, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    return toV8(window, creationContext, isolate);
}
