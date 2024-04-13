bool RenderBox::sizesToIntrinsicLogicalWidth(LogicalWidthType widthType) const
{
    if (isFloating() || (isInlineBlockOrInlineTable() && !isHTMLMarquee()))
        return true;

    Length logicalWidth = (widthType == MaxLogicalWidth) ? style()->logicalMaxWidth() : style()->logicalWidth();
    if (logicalWidth.type() == Intrinsic)
        return true;

    if (parent()->style()->overflowX() == OMARQUEE) {
        EMarqueeDirection dir = parent()->style()->marqueeDirection();
        if (dir == MAUTO || dir == MFORWARD || dir == MBACKWARD || dir == MLEFT || dir == MRIGHT)
            return true;
    }

    if (parent()->isDeprecatedFlexibleBox()
            && (parent()->style()->boxOrient() == HORIZONTAL || parent()->style()->boxAlign() != BSTRETCH))
        return true;

    if (logicalWidth.type() == Auto && !(parent()->isDeprecatedFlexibleBox() && parent()->style()->boxOrient() == VERTICAL && parent()->style()->boxAlign() == BSTRETCH) && node() && (node()->hasTagName(inputTag) || node()->hasTagName(selectTag) || node()->hasTagName(buttonTag) || node()->hasTagName(textareaTag) || node()->hasTagName(legendTag)))
        return true;

    return false;
}
