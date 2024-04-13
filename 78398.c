void ScriptController::clearScriptObjects()
{
    PluginObjectMap::iterator it = m_pluginObjects.begin();
    for (; it != m_pluginObjects.end(); ++it) {
        _NPN_UnregisterObject(it->value);
        _NPN_ReleaseObject(it->value);
    }
    m_pluginObjects.clear();

    if (m_windowScriptNPObject) {
        disposeUnderlyingV8Object(m_windowScriptNPObject);
        _NPN_ReleaseObject(m_windowScriptNPObject);
        m_windowScriptNPObject = 0;
    }
}
