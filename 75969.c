void RenderBox::computePositionedLogicalWidth()
{
    if (isReplaced()) {
        computePositionedLogicalWidthReplaced();
        return;
    }






    const RenderBoxModelObject* containerBlock = toRenderBoxModelObject(container());
    
    const int containerLogicalWidth = containingBlockLogicalWidthForPositioned(containerBlock);

    TextDirection containerDirection = (document()->inQuirksMode()) ? parent()->style()->direction() : containerBlock->style()->direction();

    bool isHorizontal = isHorizontalWritingMode();
    const int bordersPlusPadding = borderAndPaddingLogicalWidth();
    const Length marginLogicalLeft = isHorizontal ? style()->marginLeft() : style()->marginTop();
    const Length marginLogicalRight = isHorizontal ? style()->marginRight() : style()->marginBottom();
    int& marginLogicalLeftAlias = isHorizontal ? m_marginLeft : m_marginTop;
    int& marginLogicalRightAlias = isHorizontal ? m_marginRight : m_marginBottom;

    Length logicalLeft = style()->logicalLeft();
    Length logicalRight = style()->logicalRight();

    /*---------------------------------------------------------------------------*\
     * For the purposes of this section and the next, the term "static position"
     * (of an element) refers, roughly, to the position an element would have had
     * in the normal flow. More precisely:
     *
     * * The static position for 'left' is the distance from the left edge of the
     *   containing block to the left margin edge of a hypothetical box that would
     *   have been the first box of the element if its 'position' property had
     *   been 'static' and 'float' had been 'none'. The value is negative if the
     *   hypothetical box is to the left of the containing block.
     * * The static position for 'right' is the distance from the right edge of the
     *   containing block to the right margin edge of the same hypothetical box as
     *   above. The value is positive if the hypothetical box is to the left of the
     *   containing block's edge.
     *
     * But rather than actually calculating the dimensions of that hypothetical box,
     * user agents are free to make a guess at its probable position.
     *
     * For the purposes of calculating the static position, the containing block of
     * fixed positioned elements is the initial containing block instead of the
     * viewport, and all scrollable boxes should be assumed to be scrolled to their
     * origin.
    \*---------------------------------------------------------------------------*/

    computeInlineStaticDistance(logicalLeft, logicalRight, this, containerBlock, containerLogicalWidth, containerDirection);
    
    int logicalWidthResult;
    int logicalLeftResult;
    computePositionedLogicalWidthUsing(style()->logicalWidth(), containerBlock, containerDirection,
                                       containerLogicalWidth, bordersPlusPadding,
                                       logicalLeft, logicalRight, marginLogicalLeft, marginLogicalRight,
                                       logicalWidthResult, marginLogicalLeftAlias, marginLogicalRightAlias, logicalLeftResult);
    setLogicalWidth(logicalWidthResult);
    setLogicalLeft(logicalLeftResult);

    if (!style()->logicalMaxWidth().isUndefined()) {
        int maxLogicalWidth;
        int maxMarginLogicalLeft;
        int maxMarginLogicalRight;
        int maxLogicalLeftPos;

        computePositionedLogicalWidthUsing(style()->logicalMaxWidth(), containerBlock, containerDirection,
                                           containerLogicalWidth, bordersPlusPadding,
                                           logicalLeft, logicalRight, marginLogicalLeft, marginLogicalRight,
                                           maxLogicalWidth, maxMarginLogicalLeft, maxMarginLogicalRight, maxLogicalLeftPos);

        if (logicalWidth() > maxLogicalWidth) {
            setLogicalWidth(maxLogicalWidth);
            marginLogicalLeftAlias = maxMarginLogicalLeft;
            marginLogicalRightAlias = maxMarginLogicalRight;
            setLogicalLeft(maxLogicalLeftPos);
        }
    }

    if (!style()->logicalMinWidth().isZero()) {
        int minLogicalWidth;
        int minMarginLogicalLeft;
        int minMarginLogicalRight;
        int minLogicalLeftPos;

        computePositionedLogicalWidthUsing(style()->logicalMinWidth(), containerBlock, containerDirection,
                                           containerLogicalWidth, bordersPlusPadding,
                                           logicalLeft, logicalRight, marginLogicalLeft, marginLogicalRight,
                                           minLogicalWidth, minMarginLogicalLeft, minMarginLogicalRight, minLogicalLeftPos);

        if (logicalWidth() < minLogicalWidth) {
            setLogicalWidth(minLogicalWidth);
            marginLogicalLeftAlias = minMarginLogicalLeft;
            marginLogicalRightAlias = minMarginLogicalRight;
            setLogicalLeft(minLogicalLeftPos);
        }
    }

    if (stretchesToMinIntrinsicLogicalWidth() && logicalWidth() < minPreferredLogicalWidth() - bordersPlusPadding) {
        computePositionedLogicalWidthUsing(Length(minPreferredLogicalWidth() - bordersPlusPadding, Fixed), containerBlock, containerDirection,
                                           containerLogicalWidth, bordersPlusPadding,
                                           logicalLeft, logicalRight, marginLogicalLeft, marginLogicalRight,
                                           logicalWidthResult, marginLogicalLeftAlias, marginLogicalRightAlias, logicalLeftResult);
        setLogicalWidth(logicalWidthResult);
        setLogicalLeft(logicalLeftResult);
    }

    setLogicalWidth(logicalWidth() + bordersPlusPadding);
}
