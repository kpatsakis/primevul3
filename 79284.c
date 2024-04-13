inline size_t SearchBuffer::append(const UChar* characters, size_t length)
{
    ASSERT(length);
    if (!(m_options & CaseInsensitive)) {
        append(characters[0], true);
        return 1;
    }
    const int maxFoldedCharacters = 16; // sensible maximum is 3, this should be more than enough
    UChar foldedCharacters[maxFoldedCharacters];
    bool error;
    int numFoldedCharacters = foldCase(foldedCharacters, maxFoldedCharacters, characters, 1, &error);
    ASSERT(!error);
    ASSERT(numFoldedCharacters);
    ASSERT(numFoldedCharacters <= maxFoldedCharacters);
    if (!error && numFoldedCharacters) {
        numFoldedCharacters = min(numFoldedCharacters, maxFoldedCharacters);
        append(foldedCharacters[0], true);
        for (int i = 1; i < numFoldedCharacters; ++i)
            append(foldedCharacters[i], false);
    }
    return 1;
}
