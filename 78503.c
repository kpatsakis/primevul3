    void computeAvailableWidthFromLeftAndRight()
    {
        m_availableWidth = max(0.0f, m_right - m_left) + m_overhangWidth;
    }
