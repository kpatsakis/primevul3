void ScriptController::clearForClose(bool destroyGlobal)
{
    m_windowShell->clearForClose(destroyGlobal);
    for (IsolatedWorldMap::iterator iter = m_isolatedWorlds.begin(); iter != m_isolatedWorlds.end(); ++iter)
        iter->value->clearForClose(destroyGlobal);
    V8GCController::hintForCollectGarbage();
}
