Cache::LRUList* Cache::lruListFor(CachedResource* resource)
{
    unsigned accessCount = max(resource->accessCount(), 1U);
    unsigned queueIndex = fastLog2(resource->size() / accessCount);
#ifndef NDEBUG
    resource->m_lruIndex = queueIndex;
#endif
    if (m_allResources.size() <= queueIndex)
        m_allResources.grow(queueIndex + 1);
    return &m_allResources[queueIndex];
}
