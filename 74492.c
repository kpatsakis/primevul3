        virtual ~PendingCommentCallback() 
        {
            xmlFree(s);
        }
