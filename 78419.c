bool ScriptController::setContextDebugId(int debugId)
{
    ASSERT(debugId > 0);
    if (!m_windowShell->isContextInitialized())
        return false;
    v8::HandleScope scope;
    v8::Local<v8::Context> context = m_windowShell->context();
    return V8PerContextDebugData::setContextDebugData(context, "page", debugId);
}
