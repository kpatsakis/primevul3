bool FrameLoader::willLoadMediaElementURL(KURL& url)
{
    ResourceRequest request(url);

    unsigned long identifier;
    ResourceError error;
    requestFromDelegate(request, identifier, error);
    notifier()->sendRemainingDelegateMessages(m_documentLoader.get(), identifier, ResourceResponse(url, String(), -1, String(), String()), 0, -1, -1, error);

    url = request.url();

    return error.isNull();
}
