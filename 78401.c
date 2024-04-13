int ScriptController::contextDebugId(v8::Handle<v8::Context> context)
{
    return V8PerContextDebugData::contextDebugId(context);
}
