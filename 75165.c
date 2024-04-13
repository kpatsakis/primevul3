void Cache::removeFromLiveResourcesSize(CachedResource* resource)
{
    m_liveSize -= resource->size();
    m_deadSize += resource->size();
}
