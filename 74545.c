    static PassRefPtr<HashChangeEventTask> create(PassRefPtr<Document> document)
    {
        return adoptRef(new HashChangeEventTask(document));
    }
