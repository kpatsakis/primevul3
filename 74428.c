    void appendErrorCallback(XMLTokenizer::ErrorType type, const char* message, int lineNumber, int columnNumber)
    {
        PendingErrorCallback* callback = new PendingErrorCallback;
        
        callback->message = strdup(message);
        callback->type = type;
        callback->lineNumber = lineNumber;
        callback->columnNumber = columnNumber;
        
        m_callbacks.append(callback);
    }
