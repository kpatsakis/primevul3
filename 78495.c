static inline LayoutUnit borderPaddingMarginEnd(RenderInline* child)
{
    return child->marginEnd() + child->paddingEnd() + child->borderEnd();
}
