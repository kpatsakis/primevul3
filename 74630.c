void FrameLoader::scrollToAnchor(const KURL& url)
{
    ASSERT(equalIgnoringFragmentIdentifier(url, m_URL));
    if (equalIgnoringFragmentIdentifier(url, m_URL) && !equalIgnoringNullity(url.fragmentIdentifier(), m_URL.fragmentIdentifier())) {
        Document* currentDocument = frame()->document();
        currentDocument->postTask(HashChangeEventTask::create(currentDocument));
    }
    
    m_URL = url;
    history()->updateForAnchorScroll();

    m_frame->eventHandler()->stopAutoscrollTimer();
    started();
    if (FrameView* view = m_frame->view())
        view->scrollToFragment(m_URL);

    m_isComplete = false;
    checkCompleted();
}
