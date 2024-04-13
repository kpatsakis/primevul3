void FrameLoader::checkCompleted()
{
    m_shouldCallCheckCompleted = false;

    if (m_frame->view())
        m_frame->view()->checkStopDelayingDeferredRepaints();

    if (!allChildrenAreComplete())
        return;

    if (m_isComplete)
        return;

    if (m_frame->document()->parsing())
        return;

    if (numRequests(m_frame->document()))
        return;

    m_isComplete = true;

    RefPtr<Frame> protect(m_frame);
    checkCallImplicitClose(); // if we didn't do it before

    m_frame->redirectScheduler()->startTimer();

    completed();
    if (m_frame->page())
        checkLoadComplete();
}
