void FrameLoader::detachViewsAndDocumentLoader()
{
    m_client->detachedFromParent2();
    setDocumentLoader(0);
    m_client->detachedFromParent3();
}
