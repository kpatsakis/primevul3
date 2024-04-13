    void appendCharactersCallback(const xmlChar* s, int len)
    {
        PendingCharactersCallback* callback = new PendingCharactersCallback;
        
        callback->s = xmlStrndup(s, len);
        callback->len = len;
        
        m_callbacks.append(callback);
    }
