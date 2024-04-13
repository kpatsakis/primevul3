    void commit()
    {
        m_committedWidth += m_uncommittedWidth;
        m_uncommittedWidth = 0;
    }
