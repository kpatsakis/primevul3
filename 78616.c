ResourceFetcher* XSLStyleSheet::fetcher()
{
    Document* document = ownerDocument();
    if (!document)
        return 0;
    return document->fetcher();
}
