void FrameLoader::open(CachedPage& cachedPage)
{
    ASSERT(!m_frame->tree()->parent());
    ASSERT(m_frame->page());
    ASSERT(m_frame->page()->mainFrame() == m_frame);

    m_frame->redirectScheduler()->cancel();

    closeURL();
    
    if (m_frame->script()->isEnabled()) {
        m_frame->setJSStatusBarText(String());
        m_frame->setJSDefaultStatusBarText(String());
    }

    cachedPage.restore(m_frame->page());

    checkCompleted();
}
