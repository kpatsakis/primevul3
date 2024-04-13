void ScriptController::disableEval(const String& errorMessage)
{
    if (!m_windowShell->isContextInitialized())
        return;
    v8::HandleScope handleScope(m_isolate);
    v8::Local<v8::Context> v8Context = m_windowShell->context();
    v8Context->AllowCodeGenerationFromStrings(false);
    v8Context->SetErrorMessageForCodeGenerationFromStrings(v8String(errorMessage, m_isolate));
}
