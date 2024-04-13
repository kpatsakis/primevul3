    void appendStartElementNSCallback(const xmlChar* xmlLocalName, const xmlChar* xmlPrefix, const xmlChar* xmlURI, int nb_namespaces,
                                      const xmlChar** namespaces, int nb_attributes, int nb_defaulted, const xmlChar** attributes)
    {
        PendingStartElementNSCallback* callback = new PendingStartElementNSCallback;
        
        callback->xmlLocalName = xmlStrdup(xmlLocalName);
        callback->xmlPrefix = xmlStrdup(xmlPrefix);
        callback->xmlURI = xmlStrdup(xmlURI);
        callback->nb_namespaces = nb_namespaces;
        callback->namespaces = static_cast<xmlChar**>(xmlMalloc(sizeof(xmlChar*) * nb_namespaces * 2));
        for (int i = 0; i < nb_namespaces * 2 ; i++)
            callback->namespaces[i] = xmlStrdup(namespaces[i]);
        callback->nb_attributes = nb_attributes;
        callback->nb_defaulted = nb_defaulted;
        callback->attributes = static_cast<xmlChar**>(xmlMalloc(sizeof(xmlChar*) * nb_attributes * 5));
        for (int i = 0; i < nb_attributes; i++) {
            
            for (int j = 0; j < 3; j++)
                callback->attributes[i * 5 + j] = xmlStrdup(attributes[i * 5 + j]);
            
            int len = attributes[i * 5 + 4] - attributes[i * 5 + 3];

            callback->attributes[i * 5 + 3] = xmlStrndup(attributes[i * 5 + 3], len);
            callback->attributes[i * 5 + 4] = callback->attributes[i * 5 + 3] + len;
        }
        
        m_callbacks.append(callback);
    }
