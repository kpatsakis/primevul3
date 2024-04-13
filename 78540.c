static LayoutUnit inlineLogicalWidth(RenderObject* child, bool start = true, bool end = true)
{
    unsigned lineDepth = 1;
    LayoutUnit extraWidth = 0;
    RenderObject* parent = child->parent();
    while (parent->isRenderInline() && lineDepth++ < cMaxLineDepth) {
        RenderInline* parentAsRenderInline = toRenderInline(parent);
        if (!isEmptyInline(parentAsRenderInline)) {
            if (start && shouldAddBorderPaddingMargin(child->previousSibling(), start))
                extraWidth += borderPaddingMarginStart(parentAsRenderInline);
            if (end && shouldAddBorderPaddingMargin(child->nextSibling(), end))
                extraWidth += borderPaddingMarginEnd(parentAsRenderInline);
            if (!start && !end)
                return extraWidth;
        }
        child = parent;
        parent = child->parent();
    }
    return extraWidth;
}
