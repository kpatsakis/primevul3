static inline RenderText* firstRenderTextInFirstLetter(RenderObject* firstLetter)
{
    if (!firstLetter)
        return 0;

    for (RenderObject* current = firstLetter->firstChild(); current; current = current->nextSibling()) {
        if (current->isText())
            return toRenderText(current);
    }
    return 0;
}
