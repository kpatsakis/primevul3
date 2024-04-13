void RenderBox::layout()
{
    ASSERT(needsLayout());

    RenderObject* child = firstChild();
    if (!child) {
        setNeedsLayout(false);
        return;
    }

    LayoutStateMaintainer statePusher(view(), this, locationOffset(), style()->isFlippedBlocksWritingMode());
    while (child) {
        child->layoutIfNeeded();
        ASSERT(!child->needsLayout());
        child = child->nextSibling();
    }
    statePusher.pop();
    setNeedsLayout(false);
}
