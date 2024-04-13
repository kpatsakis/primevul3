    void callAndRemoveFirstCallback(XMLTokenizer* tokenizer)
    {
        OwnPtr<PendingCallback> callback(m_callbacks.first());
        m_callbacks.removeFirst();
        callback->call(tokenizer);
    }
