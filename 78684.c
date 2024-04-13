    void progressStarted()
    {
        ASSERT(m_frame->page());
        if (!m_inProgress)
            m_frame->page()->progress().progressStarted(m_frame);
        m_inProgress = true;
    }
