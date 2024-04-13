        virtual void call(XMLTokenizer* tokenizer) 
        {
            tokenizer->handleError(type, message, lineNumber, columnNumber);
        }
