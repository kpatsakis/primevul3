        virtual void call(XMLTokenizer* tokenizer) {
            tokenizer->startElementNs(xmlLocalName, xmlPrefix, xmlURI, 
                                      nb_namespaces, const_cast<const xmlChar**>(namespaces),
                                      nb_attributes, nb_defaulted, const_cast<const xmlChar**>(attributes));
        }
