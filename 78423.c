NPObject* ScriptController::windowScriptNPObject()
{
    if (m_windowScriptNPObject)
        return m_windowScriptNPObject;

    if (canExecuteScripts(NotAboutToExecuteScript)) {
        m_windowScriptNPObject = createScriptObject(m_frame);
        _NPN_RegisterObject(m_windowScriptNPObject, 0);
    } else {
        m_windowScriptNPObject = createNoScriptObject();
    }
    return m_windowScriptNPObject;
}
