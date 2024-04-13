IntRect RenderBox::clippedOverflowRectForRepaint(RenderBoxModelObject* repaintContainer)
{
    if (style()->visibility() != VISIBLE && !enclosingLayer()->hasVisibleContent())
        return IntRect();

    IntRect r = visualOverflowRect();

    RenderView* v = view();
    if (v) {
        r.move(v->layoutDelta());
    }
    
    if (style()) {
        if (style()->hasAppearance())
            theme()->adjustRepaintRect(this, r);

        if (v) {
            ASSERT(style()->outlineSize() <= v->maximalOutlineSize());
            r.inflate(v->maximalOutlineSize());
        }
    }
    
    computeRectForRepaint(repaintContainer, r);
    return r;
}
