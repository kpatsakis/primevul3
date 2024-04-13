bool Element::rendererIsFocusable() const
{
    if (isInCanvasSubtree()) {
        const Element* e = this;
        while (e && !e->hasLocalName(canvasTag))
            e = e->parentElement();
        ASSERT(e);
        return e->renderer() && e->renderer()->style()->visibility() == VISIBLE;
    }

    if (renderer()) {
        ASSERT(!renderer()->needsLayout());
    } else {
        ASSERT(!document()->childNeedsStyleRecalc());
    }

    if (!renderer() || renderer()->style()->visibility() != VISIBLE)
        return false;

    return true;
}
