static NPObject* createScriptObject(Frame* frame)
{
    v8::HandleScope handleScope;
    v8::Handle<v8::Context> v8Context = ScriptController::mainWorldContext(frame);
    if (v8Context.IsEmpty())
        return createNoScriptObject();

    v8::Context::Scope scope(v8Context);
    DOMWindow* window = frame->domWindow();
    v8::Handle<v8::Value> global = toV8(window, v8::Handle<v8::Object>(), v8Context->GetIsolate());
    ASSERT(global->IsObject());

    return npCreateV8ScriptObject(0, v8::Handle<v8::Object>::Cast(global), window);
}
