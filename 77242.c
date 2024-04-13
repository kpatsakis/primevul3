void HarfBuzzShaperBase::setNormalizedBuffer(NormalizeMode normalizeMode)
{

    icu::UnicodeString normalizedString;
    UErrorCode error = U_ZERO_ERROR;

    for (int i = 0; i < m_run.length(); ++i) {
        UChar ch = m_run[i];
        if (::ublock_getCode(ch) == UBLOCK_COMBINING_DIACRITICAL_MARKS) {
            icu::Normalizer::normalize(icu::UnicodeString(m_run.characters(),
                                       m_run.length()), UNORM_NFC, 0 /* no options */,
                                       normalizedString, error);
            if (U_FAILURE(error))
                normalizedString.remove();
            break;
        }
    }

    const UChar* sourceText;
    if (normalizedString.isEmpty()) {
        m_normalizedBufferLength = m_run.length();
        sourceText = m_run.characters();
    } else {
        m_normalizedBufferLength = normalizedString.length();
        sourceText = normalizedString.getBuffer();
    }

    m_normalizedBuffer = adoptArrayPtr(new UChar[m_normalizedBufferLength + 1]);
    normalizeSpacesAndMirrorChars(sourceText, m_normalizedBuffer.get(), m_normalizedBufferLength, normalizeMode);
}
