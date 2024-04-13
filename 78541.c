static inline bool isCollapsibleSpace(UChar character, RenderText* renderer)
{
    if (character == ' ' || character == '\t' || character == softHyphen)
        return true;
    if (character == '\n')
        return !renderer->style()->preserveNewline();
    return false;
}
