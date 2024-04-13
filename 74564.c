static inline bool frameContainsWMLContent(Frame* frame)
{
    Document* document = frame ? frame->document() : 0;
    if (!document)
        return false;

    return document->containsWMLContent() || document->isWMLDocument();
}
