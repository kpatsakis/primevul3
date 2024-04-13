V8WindowShell* ScriptController::windowShell(DOMWrapperWorld* world)
{
    ASSERT(world);

    V8WindowShell* shell = 0;
    if (world->isMainWorld())
        shell = m_windowShell.get();
    else {
        IsolatedWorldMap::iterator iter = m_isolatedWorlds.find(world->worldId());
        if (iter != m_isolatedWorlds.end())
            shell = iter->value.get();
        else {
            OwnPtr<V8WindowShell> isolatedWorldShell = V8WindowShell::create(m_frame, world, m_isolate);
            shell = isolatedWorldShell.get();
            m_isolatedWorlds.set(world->worldId(), isolatedWorldShell.release());
        }
    }
    if (!shell->isContextInitialized() && shell->initializeIfNeeded()) {
        if (world->isMainWorld()) {
            m_frame->loader()->dispatchDidClearWindowObjectInWorld(existingWindowShellWorkaroundWorld());
        } else
            m_frame->loader()->dispatchDidClearWindowObjectInWorld(world);
    }
    return shell;
}
