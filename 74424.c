    void appendCDATABlockCallback(const xmlChar* s, int len)
    {
        PendingCDATABlockCallback* callback = new PendingCDATABlockCallback;
        
        callback->s = xmlStrndup(s, len);
        callback->len = len;
        
        m_callbacks.append(callback);        
    }
