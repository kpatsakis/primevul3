static UStringSearch* searcher()
{
    static UStringSearch* searcher = createSearcher();
    return searcher;
}
