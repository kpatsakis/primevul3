void FrameLoader::setDocumentLoader(DocumentLoader* loader)
{
    if (!loader && !m_documentLoader)
        return;
    
    ASSERT(loader != m_documentLoader);
    ASSERT(!loader || loader->frameLoader() == this);

    m_client->prepareForDataSourceReplacement();
    detachChildren();
    if (m_documentLoader)
        m_documentLoader->detachFromFrame();

    m_documentLoader = loader;
}
