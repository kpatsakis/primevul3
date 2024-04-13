void Cache::dumpLRULists(bool includeLive) const
{
    printf("LRU-SP lists in eviction order (Kilobytes decoded, Kilobytes encoded, Access count, Referenced):\n");

    int size = m_allResources.size();
    for (int i = size - 1; i >= 0; i--) {
        printf("\n\nList %d: ", i);
        CachedResource* current = m_allResources[i].m_tail;
        while (current) {
            CachedResource* prev = current->m_prevInAllResourcesList;
            if (includeLive || !current->referenced())
                printf("(%.1fK, %.1fK, %uA, %dR); ", current->decodedSize() / 1024.0f, current->encodedSize() / 1024.0f, current->accessCount(), current->referenced());
            current = prev;
        }
    }
}
