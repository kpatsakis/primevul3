void FrameLoader::checkLoadComplete()
{
    ASSERT(m_client->hasWebView());
    
    m_shouldCallCheckLoadComplete = false;

    if (Page* page = m_frame->page())
        page->mainFrame()->loader()->recursiveCheckLoadComplete();
}
