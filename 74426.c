    void appendCommentCallback(const xmlChar* s)
    {
        PendingCommentCallback* callback = new PendingCommentCallback;
        
        callback->s = xmlStrdup(s);
        
        m_callbacks.append(callback);        
    }
