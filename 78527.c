static inline int findFirstTrailingSpace(RenderText* lastText, const CharacterType* characters, int start, int stop)
{
    int firstSpace = stop;
    while (firstSpace > start) {
        UChar current = characters[firstSpace - 1];
        if (!isCollapsibleSpace(current, lastText))
            break;
        firstSpace--;
    }

    return firstSpace;
}
