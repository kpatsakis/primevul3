void ScriptController::collectIsolatedContexts(Vector<std::pair<ScriptState*, SecurityOrigin*> >& result)
{
    v8::HandleScope handleScope;
    for (IsolatedWorldMap::iterator it = m_isolatedWorlds.begin(); it != m_isolatedWorlds.end(); ++it) {
        V8WindowShell* isolatedWorldShell = it->value.get();
        SecurityOrigin* origin = isolatedWorldShell->world()->isolatedWorldSecurityOrigin();
        if (!origin)
            continue;
        v8::Local<v8::Context> v8Context = isolatedWorldShell->context();
        if (v8Context.IsEmpty())
            continue;
        ScriptState* scriptState = ScriptState::forContext(v8Context);
        result.append(std::pair<ScriptState*, SecurityOrigin*>(scriptState, origin));
    }
}
