    void progressCompleted()
    {
        ASSERT(m_inProgress);
        ASSERT(m_frame->page());
        m_inProgress = false;
        m_frame->page()->progress().progressCompleted(m_frame);
    }
