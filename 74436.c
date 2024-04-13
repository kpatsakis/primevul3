        virtual void call(XMLTokenizer* tokenizer) 
        {
            tokenizer->processingInstruction(target, data);
        }
