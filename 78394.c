void ScriptController::cleanupScriptObjectsForPlugin(Widget* nativeHandle)
{
    PluginObjectMap::iterator it = m_pluginObjects.find(nativeHandle);
    if (it == m_pluginObjects.end())
        return;
    _NPN_UnregisterObject(it->value);
    _NPN_ReleaseObject(it->value);
    m_pluginObjects.remove(it);
}
