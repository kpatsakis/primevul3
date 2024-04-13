void ScriptController::enableEval()
{
    if (!m_windowShell->isContextInitialized())
        return;
    v8::HandleScope handleScope(m_isolate);
    m_windowShell->context()->AllowCodeGenerationFromStrings(true);
}
