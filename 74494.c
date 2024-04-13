        virtual ~PendingProcessingInstructionCallback() 
        {
            xmlFree(target);
            xmlFree(data);
        }
