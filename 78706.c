    ~FrameProgressTracker()
    {
        ASSERT(!m_inProgress || m_frame->page());
        if (m_inProgress)
            m_frame->page()->progress().progressCompleted(m_frame);
    }
