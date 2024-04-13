void FrameLoader::reload(ReloadPolicy reloadPolicy, const KURL& overrideURL, const String& overrideEncoding)
{
    DocumentLoader* documentLoader = activeDocumentLoader();
    if (!documentLoader)
        return;

    if (m_state == FrameStateProvisional)
        insertDummyHistoryItem();
    frame()->loader()->history()->saveDocumentAndScrollState();

    ResourceRequest request = documentLoader->request();
    request.setCachePolicy(UseProtocolCachePolicy);
    if (!overrideURL.isEmpty())
        request.setURL(overrideURL);
    else if (!documentLoader->unreachableURL().isEmpty())
        request.setURL(documentLoader->unreachableURL());

    FrameLoadType type = reloadPolicy == EndToEndReload ? FrameLoadTypeReloadFromOrigin : FrameLoadTypeReload;
    NavigationAction action(request, type, request.httpMethod() == "POST");
    loadWithNavigationAction(request, action, type, 0, SubstituteData(), overrideEncoding);
}
