String FrameLoader::userAgent(const KURL& url) const
{
    return m_client->userAgent(url);
}
