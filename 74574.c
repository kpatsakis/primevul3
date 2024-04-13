void FrameLoader::init()
{
    m_isDisplayingInitialEmptyDocument = false;
    m_creatingInitialEmptyDocument = true;
    setPolicyDocumentLoader(m_client->createDocumentLoader(ResourceRequest(KURL(ParsedURLString, "")), SubstituteData()).get());
    setProvisionalDocumentLoader(m_policyDocumentLoader.get());
    setState(FrameStateProvisional);
    m_provisionalDocumentLoader->setResponse(ResourceResponse(KURL(), "text/html", 0, String(), String()));
    m_provisionalDocumentLoader->finishedLoading();
    begin(KURL(), false);
    end();
    m_frame->document()->cancelParsing();
    m_creatingInitialEmptyDocument = false;
    m_didCallImplicitClose = true;
}
