void FrameLoader::didFirstLayout()
{
    if (Page* page = m_frame->page())
        if (isBackForwardLoadType(m_loadType) && page->backForwardList())
            history()->restoreScrollPositionAndViewState();

    m_firstLayoutDone = true;
    m_client->dispatchDidFirstLayout();
}
