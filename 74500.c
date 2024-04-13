void FrameLoader::addExtraFieldsToMainResourceRequest(ResourceRequest& request)
{
    addExtraFieldsToRequest(request, m_loadType, true, false);
}
