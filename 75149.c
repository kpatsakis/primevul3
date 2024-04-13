void Cache::addToLiveResourcesSize(CachedResource* resource)
{
    m_liveSize += resource->size();
    m_deadSize -= resource->size();
}
