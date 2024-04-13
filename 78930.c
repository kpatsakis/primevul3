    void push()
    {
        if (m_pushedStyleResolver)
            return;
        m_pushedStyleResolver = m_parent->document()->styleResolver();
        m_pushedStyleResolver->pushParentElement(m_parent);
    }
