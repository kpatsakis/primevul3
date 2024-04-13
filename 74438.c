        virtual void call(XMLTokenizer* tokenizer)
        {
            tokenizer->internalSubset(name, externalID, systemID);
        }
