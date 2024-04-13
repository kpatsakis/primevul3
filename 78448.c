    virtual void didCreateFrame(WebFrame* parent, WebFrame* child)
    {
        m_frameCount++;
        if (!m_parent)
            m_parent = parent;
    }
