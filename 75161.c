void Cache::remove(CachedResource* resource)
{
    if (resource->inCache()) {
        m_resources.remove(resource->url());
        resource->setInCache(false);

        removeFromLRUList(resource);
        removeFromLiveDecodedResourcesList(resource);
        
        HashSet<DocLoader*>::iterator end = m_docLoaders.end();
        for (HashSet<DocLoader*>::iterator itr = m_docLoaders.begin(); itr != end; ++itr)
            (*itr)->removeCachedResource(resource);

        int delta = -static_cast<int>(resource->size());
        if (delta)
            adjustSize(resource->referenced(), delta);
    } else
        ASSERT(m_resources.get(resource->url()) != resource);

    if (resource->canDelete())
        delete resource;
}
