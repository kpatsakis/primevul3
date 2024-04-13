void RenderBlock::addOverflowFromInlineChildren()
{
    LayoutUnit endPadding = hasOverflowClip() ? paddingEnd() : LayoutUnit();
    if (hasOverflowClip() && !endPadding && node() && node()->isRootEditableElement() && style()->isLeftToRightDirection())
        endPadding = 1;
    for (RootInlineBox* curr = firstRootBox(); curr; curr = curr->nextRootBox()) {
        addLayoutOverflow(curr->paddedLayoutOverflowRect(endPadding));
        LayoutRect visualOverflow = curr->visualOverflowRect(curr->lineTop(), curr->lineBottom());
        addContentsVisualOverflow(visualOverflow);
    }
}
