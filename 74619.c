void FrameLoader::reload(bool endToEndReload)
{
    if (!m_documentLoader)
        return;

    if (m_documentLoader->request().url().isEmpty())
        return;

    ResourceRequest initialRequest = m_documentLoader->request();

    KURL unreachableURL = m_documentLoader->unreachableURL();
    if (!unreachableURL.isEmpty())
        initialRequest.setURL(unreachableURL);
    
    RefPtr<DocumentLoader> loader = m_client->createDocumentLoader(initialRequest, SubstituteData());

    ResourceRequest& request = loader->request();

    request.setCachePolicy(ReloadIgnoringCacheData);

    if (request.httpMethod() == "POST")
        loader->setTriggeringAction(NavigationAction(request.url(), NavigationTypeFormResubmitted));

    loader->setOverrideEncoding(m_documentLoader->overrideEncoding());
    
    loadWithDocumentLoader(loader.get(), endToEndReload ? FrameLoadTypeReloadFromOrigin : FrameLoadTypeReload, 0);
}
