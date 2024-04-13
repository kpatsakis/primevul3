    void appendProcessingInstructionCallback(const xmlChar* target, const xmlChar* data)
    {
        PendingProcessingInstructionCallback* callback = new PendingProcessingInstructionCallback;
        
        callback->target = xmlStrdup(target);
        callback->data = xmlStrdup(data);
        
        m_callbacks.append(callback);
    }
