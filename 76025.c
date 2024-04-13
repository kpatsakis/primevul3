void RenderBox::positionLineBox(InlineBox* box)
{
    if (isPositioned()) {
        bool wasInline = style()->isOriginalDisplayInlineType();
        if (wasInline) {
            layer()->setStaticInlinePosition(lroundf(box->logicalLeft()));
            if (style()->hasStaticInlinePosition(box->isHorizontal()))
                setChildNeedsLayout(true, false); // Just go ahead and mark the positioned object as needing layout, so it will update its position properly.
        } else {
            layer()->setStaticBlockPosition(box->logicalTop());
            if (style()->hasStaticBlockPosition(box->isHorizontal()))
                setChildNeedsLayout(true, false); // Just go ahead and mark the positioned object as needing layout, so it will update its position properly.
        }

        box->remove();
        box->destroy(renderArena());
    } else if (isReplaced()) {
        setLocation(roundedIntPoint(FloatPoint(box->x(), box->y())));
        m_inlineBoxWrapper = box;
    }
}
