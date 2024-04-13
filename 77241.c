static void normalizeSpacesAndMirrorChars(const UChar* source, UChar* destination, int length, HarfBuzzShaperBase::NormalizeMode normalizeMode)
{
    int position = 0;
    bool error = false;
    while (position < length) {
        UChar32 character;
        int nextPosition = position;
        U16_NEXT(source, nextPosition, length, character);
        if (Font::treatAsSpace(character))
            character = ' ';
        else if (Font::treatAsZeroWidthSpace(character))
            character = zeroWidthSpace;
        else if (normalizeMode == HarfBuzzShaperBase::NormalizeMirrorChars)
            character = u_charMirror(character);
        U16_APPEND(destination, position, length, character, error);
        ASSERT(!error);
        position = nextPosition;
    }
}
