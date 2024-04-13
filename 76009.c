LayoutSize RenderBox::offsetFromContainer(RenderObject* o, const LayoutPoint& point) const
{
    ASSERT(o == container());

    LayoutSize offset;    
    if (isRelPositioned())
        offset += relativePositionOffset();

    if (!isInline() || isReplaced()) {
        if (style()->position() != AbsolutePosition && style()->position() != FixedPosition) {
            if (o->hasColumns()) {
                LayoutRect columnRect(frameRect());
                toRenderBlock(o)->adjustStartEdgeForWritingModeIncludingColumns(columnRect);
                offset += LayoutSize(columnRect.location().x(), columnRect.location().y());
                columnRect.moveBy(point);
                o->adjustForColumns(offset, columnRect.location());
            } else
                offset += locationOffsetIncludingFlipping();
        } else
            offset += locationOffsetIncludingFlipping();
    }

    if (o->hasOverflowClip())
        offset -= toRenderBox(o)->layer()->scrolledContentOffset();

    if (style()->position() == AbsolutePosition && o->isRelPositioned() && o->isRenderInline())
        offset += toRenderInline(o)->relativePositionedInlineOffset(this);

    return offset;
}
