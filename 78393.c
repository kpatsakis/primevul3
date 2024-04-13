bool ScriptController::canExecuteScripts(ReasonForCallingCanExecuteScripts reason)
{
    if (m_frame->document() && m_frame->document()->isSandboxed(SandboxScripts)) {
        if (reason == AboutToExecuteScript)
            m_frame->document()->addConsoleMessage(SecurityMessageSource, ErrorMessageLevel, "Blocked script execution in '" + m_frame->document()->url().elidedString() + "' because the document's frame is sandboxed and the 'allow-scripts' permission is not set.");
        return false;
    }

    if (m_frame->document() && m_frame->document()->isViewSource()) {
        ASSERT(m_frame->document()->securityOrigin()->isUnique());
        return true;
    }

    Settings* settings = m_frame->settings();
    const bool allowed = m_frame->loader()->client()->allowScript(settings && settings->isScriptEnabled());
    if (!allowed && reason == AboutToExecuteScript)
        m_frame->loader()->client()->didNotAllowScript();
    return allowed;
}
