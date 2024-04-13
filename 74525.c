void FrameLoader::checkDidPerformFirstNavigation()
{
    Page* page = m_frame->page();
    if (!page)
        return;

    if (!m_didPerformFirstNavigation && page->backForwardList()->entries().size() == 1) {
        m_didPerformFirstNavigation = true;
        m_client->didPerformFirstNavigation();
    }
}
