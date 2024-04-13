void FrameLoader::load(const ResourceRequest& request, const SubstituteData& substituteData, bool lockHistory)
{
    if (m_inStopAllLoaders)
        return;
        
    m_loadType = FrameLoadTypeStandard;
    RefPtr<DocumentLoader> loader = m_client->createDocumentLoader(request, substituteData);
    if (lockHistory && m_documentLoader)
        loader->setClientRedirectSourceForHistory(m_documentLoader->didCreateGlobalHistoryEntry() ? m_documentLoader->urlForHistory() : m_documentLoader->clientRedirectSourceForHistory());
    load(loader.get());
}
