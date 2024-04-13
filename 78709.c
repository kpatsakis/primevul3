    virtual void willSendRequest(WebFrame* frame, unsigned, WebURLRequest&, const WebURLResponse&)
    {
        if (toWebFrameImpl(frame)->frame()->loader()->loadType() == WebCore::FrameLoadTypeSame)
            m_frameLoadTypeSameSeen = true;
    }
