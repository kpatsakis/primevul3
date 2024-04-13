RenderObject* TextAutosizer::nextInPreOrderSkippingDescendantsOfContainers(const RenderObject* current, const RenderObject* stayWithin)
{
    if (current == stayWithin || !isAutosizingContainer(current))
        for (RenderObject* child = current->firstChild(); child; child = child->nextSibling())
            return child;

    for (const RenderObject* ancestor = current; ancestor; ancestor = ancestor->parent()) {
        if (ancestor == stayWithin)
            return 0;
        for (RenderObject* sibling = ancestor->nextSibling(); sibling; sibling = sibling->nextSibling())
            return sibling;
    }

    return 0;
}
