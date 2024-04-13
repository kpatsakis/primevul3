void Cache::removeFromLRUList(CachedResource* resource)
{
    if (resource->accessCount() == 0)
        return;

#ifndef NDEBUG
    unsigned oldListIndex = resource->m_lruIndex;
#endif

    LRUList* list = lruListFor(resource);

#ifndef NDEBUG
    ASSERT(resource->m_lruIndex == oldListIndex);

    bool found = false;
    for (CachedResource* current = list->m_head; current; current = current->m_nextInAllResourcesList) {
        if (current == resource) {
            found = true;
            break;
        }
    }
    ASSERT(found);
#endif

    CachedResource* next = resource->m_nextInAllResourcesList;
    CachedResource* prev = resource->m_prevInAllResourcesList;
    
    if (next == 0 && prev == 0 && list->m_head != resource)
        return;
    
    resource->m_nextInAllResourcesList = 0;
    resource->m_prevInAllResourcesList = 0;
    
    if (next)
        next->m_prevInAllResourcesList = prev;
    else if (list->m_tail == resource)
        list->m_tail = prev;

    if (prev)
        prev->m_nextInAllResourcesList = next;
    else if (list->m_head == resource)
        list->m_head = next;
}
