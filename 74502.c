void FrameLoader::addExtraFieldsToSubresourceRequest(ResourceRequest& request)
{
    addExtraFieldsToRequest(request, m_loadType, false, false);
}
