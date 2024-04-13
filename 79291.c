static int collapsedSpaceLength(RenderText* renderer, int textEnd)
{
    const UChar* characters = renderer->text()->characters();
    int length = renderer->text()->length();
    for (int i = textEnd; i < length; ++i) {
        if (!renderer->style()->isCollapsibleWhiteSpace(characters[i]))
            return i - textEnd;
    }

    return length - textEnd;
}
