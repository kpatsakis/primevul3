    virtual ~WebFrameTest()
    {
        if (m_webView)
            m_webView->close();
    }
