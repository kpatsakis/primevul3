inline SearchBuffer::SearchBuffer(const String& target, FindOptions options)
    : m_target(target)
    , m_options(options)
    , m_prefixLength(0)
    , m_atBreak(true)
    , m_needsMoreContext(options & AtWordStarts)
    , m_targetRequiresKanaWorkaround(containsKanaLetters(m_target))
{
    ASSERT(!m_target.isEmpty());

    foldQuoteMarksAndSoftHyphens(m_target);

    size_t targetLength = m_target.length();
    m_buffer.reserveInitialCapacity(max(targetLength * 8, minimumSearchBufferSize));
    m_overlap = m_buffer.capacity() / 4;

    if ((m_options & AtWordStarts) && targetLength) {
        UChar32 targetFirstCharacter;
        U16_GET(m_target.characters(), 0, 0, targetLength, targetFirstCharacter);
        if (isSeparator(targetFirstCharacter)) {
            m_options &= ~AtWordStarts;
            m_needsMoreContext = false;
        }
    }

    lockSearcher();

    UStringSearch* searcher = WebCore::searcher();
    UCollator* collator = usearch_getCollator(searcher);

    UCollationStrength strength = m_options & CaseInsensitive ? UCOL_PRIMARY : UCOL_TERTIARY;
    if (ucol_getStrength(collator) != strength) {
        ucol_setStrength(collator, strength);
        usearch_reset(searcher);
    }

    UErrorCode status = U_ZERO_ERROR;
    usearch_setPattern(searcher, m_target.characters(), targetLength, &status);
    ASSERT(status == U_ZERO_ERROR);

    if (m_targetRequiresKanaWorkaround)
        normalizeCharacters(m_target.characters(), m_target.length(), m_normalizedTarget);
}
