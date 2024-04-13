void FrameLoader::clientRedirectCancelledOrFinished(bool cancelWithLoadInProgress)
{
    m_client->dispatchDidCancelClientRedirect();

    if (!cancelWithLoadInProgress)
        m_quickRedirectComing = false;

    m_sentRedirectNotification = false;
}
