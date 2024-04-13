void FrameLoader::didAccessInitialDocumentTimerFired(Timer<FrameLoader>*)
{
     m_client->didAccessInitialDocument();
 }
