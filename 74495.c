        virtual ~PendingStartElementNSCallback() {
            xmlFree(xmlLocalName);
            xmlFree(xmlPrefix);
            xmlFree(xmlURI);
            for (int i = 0; i < nb_namespaces * 2; i++)
                xmlFree(namespaces[i]);
            xmlFree(namespaces);
            for (int i = 0; i < nb_attributes; i++)
                for (int j = 0; j < 4; j++) 
                    xmlFree(attributes[i * 5 + j]);
            xmlFree(attributes);
        }
