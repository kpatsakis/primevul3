V8WindowShell* ScriptController::existingWindowShell(DOMWrapperWorld* world)
{
    ASSERT(world);

    if (world->isMainWorld())
        return m_windowShell->isContextInitialized() ? m_windowShell.get() : 0;

    if (world == existingWindowShellWorkaroundWorld())
        return m_windowShell.get();

    IsolatedWorldMap::iterator iter = m_isolatedWorlds.find(world->worldId());
    if (iter == m_isolatedWorlds.end())
        return 0;
    return iter->value->isContextInitialized() ? iter->value.get() : 0;
}
