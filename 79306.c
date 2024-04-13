static inline void foldQuoteMarksAndSoftHyphens(String& s)
{
    s.replace(hebrewPunctuationGeresh, '\'');
    s.replace(hebrewPunctuationGershayim, '"');
    s.replace(leftDoubleQuotationMark, '"');
    s.replace(leftSingleQuotationMark, '\'');
    s.replace(rightDoubleQuotationMark, '"');
    s.replace(rightSingleQuotationMark, '\'');
    s.replace(softHyphen, 0);
}
