ScriptValue ScriptController::executeScript(const ScriptSourceCode& sourceCode)
{
    if (!canExecuteScripts(AboutToExecuteScript) || isPaused())
        return ScriptValue();

    RefPtr<Frame> protect(m_frame); // Script execution can destroy the frame, and thus the ScriptController.

    return executeScriptInMainWorld(sourceCode);
}
