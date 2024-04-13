static inline bool containsKanaLetters(const String& pattern)
{
    const UChar* characters = pattern.characters();
    unsigned length = pattern.length();
    for (unsigned i = 0; i < length; ++i) {
        if (isKanaLetter(characters[i]))
            return true;
    }
    return false;
}
