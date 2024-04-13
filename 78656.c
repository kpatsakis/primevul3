void FrameLoader::didAccessInitialDocument()
{
    if (isLoadingMainFrame() && !m_didAccessInitialDocument) {
        m_didAccessInitialDocument = true;
        m_didAccessInitialDocumentTimer.startOneShot(0);
    }
}
