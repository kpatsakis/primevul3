void FrameLoader::didReceiveServerRedirectForProvisionalLoadForFrame()
{
    m_client->dispatchDidReceiveServerRedirectForProvisionalLoad();
}
