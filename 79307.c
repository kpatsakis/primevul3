static inline void foldQuoteMarksAndSoftHyphens(UChar* data, size_t length)
{
    for (size_t i = 0; i < length; ++i)
        data[i] = foldQuoteMarkOrSoftHyphen(data[i]);
}
