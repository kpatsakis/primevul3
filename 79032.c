    ~StyleResolverParentPusher()
    {

        if (!m_pushedStyleResolver)
            return;

        ASSERT(m_pushedStyleResolver == m_parent->document()->styleResolver());
        if (m_pushedStyleResolver != m_parent->document()->styleResolver())
            return;

        m_pushedStyleResolver->popParentElement(m_parent);
    }
