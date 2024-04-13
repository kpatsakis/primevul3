    virtual void didAccessInitialDocument(WebFrame* frame)
    {
        EXPECT_TRUE(!m_didAccessInitialDocument);
        m_didAccessInitialDocument = true;
    }
