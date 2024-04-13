        virtual ~PendingInternalSubsetCallback() 
        {
            xmlFree(name);
            xmlFree(externalID);
            xmlFree(systemID);
        }
