bool FrameLoader::canHandleRequest(const ResourceRequest& request)
{
    return m_client->canHandleRequest(request);
}
