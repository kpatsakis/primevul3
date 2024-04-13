inline SearchBuffer::~SearchBuffer()
{
    UErrorCode status = U_ZERO_ERROR;
    usearch_setPattern(WebCore::searcher(), &newlineCharacter, 1, &status);
    ASSERT(status == U_ZERO_ERROR);

    unlockSearcher();
}
