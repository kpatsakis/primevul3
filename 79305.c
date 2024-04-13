static inline UChar foldQuoteMarkOrSoftHyphen(UChar c)
{
    switch (c) {
        case hebrewPunctuationGershayim:
        case leftDoubleQuotationMark:
        case rightDoubleQuotationMark:
            return '"';
        case hebrewPunctuationGeresh:
        case leftSingleQuotationMark:
        case rightSingleQuotationMark:
            return '\'';
        case softHyphen:
            return 0;
        default:
            return c;
    }
}
