void Cache::pruneDeadResources()
{
    if (!m_pruneEnabled)
        return;

    unsigned capacity = deadCapacity();
    if (m_deadSize <= capacity)
        return;

    unsigned targetSize = static_cast<unsigned>(capacity * cTargetPrunePercentage); // Cut by a percentage to avoid immediately pruning again.
    int size = m_allResources.size();
    bool canShrinkLRULists = true;
    for (int i = size - 1; i >= 0; i--) {
        CachedResource* current = m_allResources[i].m_tail;
        
        while (current) {
            CachedResource* prev = current->m_prevInAllResourcesList;
            if (!current->referenced() && !current->isPreloaded() && current->isLoaded() && current->decodedSize()) {
                current->destroyDecodedData();
                
                if (m_deadSize <= targetSize)
                    return;
            }
            current = prev;
        }

        current = m_allResources[i].m_tail;
        while (current) {
            CachedResource* prev = current->m_prevInAllResourcesList;
            if (!current->referenced() && !current->isPreloaded()) {
                remove(current);

                if (m_deadSize <= targetSize)
                    return;
            }
            current = prev;
        }
            
        if (m_allResources[i].m_head)
            canShrinkLRULists = false;
        else if (canShrinkLRULists)
            m_allResources.resize(i);
    }
}
