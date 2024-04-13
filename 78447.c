    virtual void didCommitProvisionalLoad(WebFrame* frame, bool)
    {
        if (frame->dataSource()->response().url() != WebURL(URLTestHelpers::toKURL("about:blank")))
            m_commitCalled = true;
    }
