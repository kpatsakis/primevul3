void ScriptController::updateDocument()
{
    if ((!m_windowShell->isContextInitialized() || !m_windowShell->isGlobalInitialized()) && m_frame->loader()->stateMachine()->creatingInitialEmptyDocument())
        return;

    if (!initializeMainWorld())
        windowShell(mainThreadNormalWorld())->updateDocument();
}
