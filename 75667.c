static int cssIdentifierForFontSizeKeyword(int keywordSize)
{
    ASSERT_ARG(keywordSize, keywordSize);
    ASSERT_ARG(keywordSize, keywordSize <= 8);
    return CSSValueXxSmall + keywordSize - 1;
}
