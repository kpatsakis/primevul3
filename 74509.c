ResourceError FrameLoader::blockedError(const ResourceRequest& request) const
{
    return m_client->blockedError(request);
}
