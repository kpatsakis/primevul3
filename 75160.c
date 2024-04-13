void Cache::pruneLiveResources()
{
    if (!m_pruneEnabled)
        return;

    unsigned capacity = liveCapacity();
    if (m_liveSize <= capacity)
        return;

    unsigned targetSize = static_cast<unsigned>(capacity * cTargetPrunePercentage); // Cut by a percentage to avoid immediately pruning again.
    double currentTime = Frame::currentPaintTimeStamp();
    if (!currentTime) // In case prune is called directly, outside of a Frame paint.
        currentTime = WebCore::currentTime();
    
    CachedResource* current = m_liveDecodedResources.m_tail;
    while (current) {
        CachedResource* prev = current->m_prevInLiveResourcesList;
        ASSERT(current->referenced());
        if (current->isLoaded() && current->decodedSize()) {
            double elapsedTime = currentTime - current->m_lastDecodedAccessTime;
            if (elapsedTime < cMinDelayBeforeLiveDecodedPrune)
                return;

            current->destroyDecodedData();

            if (m_liveSize <= targetSize)
                return;
        }
        current = prev;
    }
}
