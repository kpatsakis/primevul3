void FrameLoader::dispatchDidClearWindowObjectInWorld(DOMWrapperWorld* world)
{
    if (!m_frame->script()->canExecuteScripts(NotAboutToExecuteScript) || !m_frame->script()->existingWindowShell(world))
        return;

    m_client->dispatchDidClearWindowObjectInWorld(world);

    if (Page* page = m_frame->page())
        page->inspectorController().didClearWindowObjectInWorld(m_frame, world);

    InspectorInstrumentation::didClearWindowObjectInWorld(m_frame, world);
}
