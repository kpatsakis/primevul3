void FrameLoader::dispatchWindowObjectAvailable()
{
    if (!m_frame->script()->isEnabled() || !m_frame->script()->existingWindowShell(mainThreadNormalWorld()))
        return;

    m_client->windowObjectCleared();

#if ENABLE(INSPECTOR)
    if (Page* page = m_frame->page()) {
        if (InspectorController* inspector = page->inspectorController())
            inspector->inspectedWindowScriptObjectCleared(m_frame);
        if (InspectorController* inspector = page->parentInspectorController())
            inspector->windowScriptObjectAvailable();
    }
#endif
}
