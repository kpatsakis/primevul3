HTMLCollection* Element::cachedHTMLCollection(CollectionType type)
{
    return hasRareData() && rareData()->nodeLists() ? rareData()->nodeLists()->cacheWithAtomicName<HTMLCollection>(type) : 0;
}
