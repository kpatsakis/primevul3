void Cache::removeFromLiveDecodedResourcesList(CachedResource* resource)
{
    if (!resource->m_inLiveDecodedResourcesList)
        return;
    resource->m_inLiveDecodedResourcesList = false;

#ifndef NDEBUG
    bool found = false;
    for (CachedResource* current = m_liveDecodedResources.m_head; current; current = current->m_nextInLiveResourcesList) {
        if (current == resource) {
            found = true;
            break;
        }
    }
    ASSERT(found);
#endif

    CachedResource* next = resource->m_nextInLiveResourcesList;
    CachedResource* prev = resource->m_prevInLiveResourcesList;
    
    if (next == 0 && prev == 0 && m_liveDecodedResources.m_head != resource)
        return;
    
    resource->m_nextInLiveResourcesList = 0;
    resource->m_prevInLiveResourcesList = 0;
    
    if (next)
        next->m_prevInLiveResourcesList = prev;
    else if (m_liveDecodedResources.m_tail == resource)
        m_liveDecodedResources.m_tail = prev;

    if (prev)
        prev->m_nextInLiveResourcesList = next;
    else if (m_liveDecodedResources.m_head == resource)
        m_liveDecodedResources.m_head = next;
}
