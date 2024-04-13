void FrameLoader::checkCompleted()
{
    RefPtr<Frame> protect(m_frame);
    m_shouldCallCheckCompleted = false;

    if (m_frame->view())
        m_frame->view()->handleLoadCompleted();

    if (m_isComplete)
        return;

    if (m_frame->document()->parsing())
        return;

    if (m_frame->document()->fetcher()->requestCount())
        return;

    if (m_frame->document()->isDelayingLoadEvent())
        return;

    if (!allChildrenAreComplete())
        return;

    m_isComplete = true;
    m_requestedHistoryItem = 0;
    m_frame->document()->setReadyState(Document::Complete);
    if (m_frame->document()->loadEventStillNeeded())
        m_frame->document()->implicitClose();

    m_frame->navigationScheduler()->startTimer();

    completed();
    if (m_frame->page())
        checkLoadComplete();

    if (m_frame->view())
        m_frame->view()->handleLoadCompleted();
}
