void RenderBox::computeRectForRepaint(RenderBoxModelObject* repaintContainer, IntRect& rect, bool fixed)
{
    if (RenderView* v = view()) {
        if (v->layoutStateEnabled() && !repaintContainer && style()->position() != FixedPosition) {
            LayoutState* layoutState = v->layoutState();

            if (layer() && layer()->transform())
                rect = layer()->transform()->mapRect(rect);

            if (style()->position() == RelativePosition && layer())
                rect.move(layer()->relativePositionOffset());

            rect.moveBy(location());
            rect.move(layoutState->m_paintOffset);
            if (layoutState->m_clipped)
                rect.intersect(layoutState->m_clipRect);
            return;
        }
    }

    if (hasReflection())
        rect.unite(reflectedRect(rect));

    if (repaintContainer == this) {
        if (repaintContainer->style()->isFlippedBlocksWritingMode())
            flipForWritingMode(rect);
        return;
    }

    bool containerSkipped;
    RenderObject* o = container(repaintContainer, &containerSkipped);
    if (!o)
        return;

    if (isWritingModeRoot() && !isPositioned())
        flipForWritingMode(rect);
    IntPoint topLeft = rect.location();
    topLeft.move(x(), y());

    EPosition position = style()->position();

    if (layer() && layer()->transform()) {
        fixed = position == FixedPosition;
        rect = layer()->transform()->mapRect(rect);
        topLeft = rect.location();
        topLeft.move(x(), y());
    } else if (position == FixedPosition)
        fixed = true;

    if (position == AbsolutePosition && o->isRelPositioned() && o->isRenderInline())
        topLeft += toRenderInline(o)->relativePositionedInlineOffset(this);
    else if (position == RelativePosition && layer()) {
        topLeft += layer()->relativePositionOffset();
    }
    
    if (o->isBlockFlow() && position != AbsolutePosition && position != FixedPosition) {
        RenderBlock* cb = toRenderBlock(o);
        if (cb->hasColumns()) {
            IntRect repaintRect(topLeft, rect.size());
            cb->adjustRectForColumns(repaintRect);
            topLeft = repaintRect.location();
            rect = repaintRect;
        }
    }

    if (o->hasOverflowClip()) {
        RenderBox* containerBox = toRenderBox(o);

        topLeft -= containerBox->layer()->scrolledContentOffset(); // For overflow:auto/scroll/hidden.

        IntRect repaintRect(topLeft, rect.size());
        IntRect boxRect(IntPoint(), containerBox->layer()->size());
        rect = intersection(repaintRect, boxRect);
        if (rect.isEmpty())
            return;
    } else
        rect.setLocation(topLeft);

    if (containerSkipped) {
        IntSize containerOffset = repaintContainer->offsetFromAncestorContainer(o);
        rect.move(-containerOffset);
        return;
    }

    o->computeRectForRepaint(repaintContainer, rect, fixed);
}
