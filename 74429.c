    void appendInternalSubsetCallback(const xmlChar* name, const xmlChar* externalID, const xmlChar* systemID)
    {
        PendingInternalSubsetCallback* callback = new PendingInternalSubsetCallback;
        
        callback->name = xmlStrdup(name);
        callback->externalID = xmlStrdup(externalID);
        callback->systemID = xmlStrdup(systemID);
        
        m_callbacks.append(callback);        
    }
