static UStringSearch* createSearcher()
{
    UErrorCode status = U_ZERO_ERROR;
    String searchCollatorName = makeString(currentSearchLocaleID(), "@collation=search");
    UStringSearch* searcher = usearch_open(&newlineCharacter, 1, &newlineCharacter, 1, searchCollatorName.utf8().data(), 0, &status);
    ASSERT(status == U_ZERO_ERROR || status == U_USING_FALLBACK_WARNING || status == U_USING_DEFAULT_WARNING);
    return searcher;
}
