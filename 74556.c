void FrameLoader::dispatchDocumentElementAvailable()
{
    m_frame->injectUserScripts(InjectAtDocumentStart);
    m_client->documentElementAvailable();
}
