static RenderObject* firstRenderObjectForDirectionalityDetermination(RenderObject* root, RenderObject* current = 0)
{
    RenderObject* next = current;
    while (current) {
        if (isIsolated(current->style()->unicodeBidi())
            && (current->isRenderInline() || current->isRenderBlock())) {
            if (current != root)
                current = 0;
            else
                current = next;
            break;
        }
        current = current->parent();
    }

    if (!current)
        current = root->firstChild();

    while (current) {
        next = 0;
        if (isIteratorTarget(current) && !(current->isText() && toRenderText(current)->isAllCollapsibleWhitespace()))
            break;

        if (!isIteratorTarget(current) && !isIsolated(current->style()->unicodeBidi()))
            next = current->firstChild();

        if (!next) {
            while (current && current != root) {
                next = current->nextSibling();
                if (next)
                    break;
                current = current->parent();
            }
        }

        if (!next)
            break;

        current = next;
    }

    return current;
}
