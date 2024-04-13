NPObject* ScriptController::createScriptObjectForPluginElement(HTMLPlugInElement* plugin)
{
    if (!canExecuteScripts(NotAboutToExecuteScript))
        return createNoScriptObject();

    v8::HandleScope handleScope;
    v8::Handle<v8::Context> v8Context = ScriptController::mainWorldContext(m_frame);
    if (v8Context.IsEmpty())
        return createNoScriptObject();
    v8::Context::Scope scope(v8Context);

    DOMWindow* window = m_frame->domWindow();
    v8::Handle<v8::Value> v8plugin = toV8(plugin, v8::Handle<v8::Object>(), v8Context->GetIsolate());
    if (!v8plugin->IsObject())
        return createNoScriptObject();

    return npCreateV8ScriptObject(0, v8::Handle<v8::Object>::Cast(v8plugin), window);
}
