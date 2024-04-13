void FrameLoader::completed()
{
    RefPtr<Frame> protect(m_frame);

    for (Frame* descendant = m_frame->tree()->traverseNext(m_frame); descendant; descendant = descendant->tree()->traverseNext(m_frame))
        descendant->navigationScheduler()->startTimer();

    if (Frame* parent = m_frame->tree()->parent())
        parent->loader()->checkCompleted();

    if (m_frame->view())
        m_frame->view()->maintainScrollPositionAtAnchor(0);
}
