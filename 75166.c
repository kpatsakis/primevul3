void Cache::resourceAccessed(CachedResource* resource)
{
    ASSERT(resource->inCache());
    
    removeFromLRUList(resource);
    
    resource->increaseAccessCount();
    
    insertInLRUList(resource);
}
