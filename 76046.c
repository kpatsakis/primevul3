void RenderBox::styleWillChange(StyleDifference diff, const RenderStyle* newStyle)
{
    s_hadOverflowClip = hasOverflowClip();

    if (style()) {
        if (diff >= StyleDifferenceRepaint && node() &&
                (node()->hasTagName(htmlTag) || node()->hasTagName(bodyTag)))
            view()->repaint();
        
        if (diff == StyleDifferenceLayout && parent() && style()->position() != newStyle->position()) {
            markContainingBlocksForLayout();
            if (style()->position() == StaticPosition)
                repaint();
            else if (newStyle->position() == AbsolutePosition || newStyle->position() == FixedPosition)
                parent()->setChildNeedsLayout(true);
            if (isFloating() && !isPositioned() && (newStyle->position() == AbsolutePosition || newStyle->position() == FixedPosition))
                removeFloatingOrPositionedChildFromBlockLists();
        }
    } else if (newStyle && isBody())
        view()->repaint();

    if (FrameView *frameView = view()->frameView()) {
        bool newStyleIsFixed = newStyle && newStyle->position() == FixedPosition;
        bool oldStyleIsFixed = style() && style()->position() == FixedPosition;
        if (newStyleIsFixed != oldStyleIsFixed) {
            if (newStyleIsFixed)
                frameView->addFixedObject();
            else
                frameView->removeFixedObject();
        }
    }

    RenderBoxModelObject::styleWillChange(diff, newStyle);
}
