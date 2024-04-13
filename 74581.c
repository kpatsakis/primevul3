bool FrameLoader::isLoadingMainFrame() const
{
    Page* page = m_frame->page();
    return page && m_frame == page->mainFrame();
}
