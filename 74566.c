bool FrameLoader::frameHasLoaded() const
{
    return m_committedFirstRealDocumentLoad || (m_provisionalDocumentLoader && !m_creatingInitialEmptyDocument); 
}
