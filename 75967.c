void RenderBox::computePositionedLogicalHeight()
{
    if (isReplaced()) {
        computePositionedLogicalHeightReplaced();
        return;
    }



    const RenderBoxModelObject* containerBlock = toRenderBoxModelObject(container());

    const int containerLogicalHeight = containingBlockLogicalHeightForPositioned(containerBlock);

    bool isHorizontal = isHorizontalWritingMode();
    bool isFlipped = style()->isFlippedBlocksWritingMode();
    const int bordersPlusPadding = borderAndPaddingLogicalHeight();
    const Length marginBefore = style()->marginBefore();
    const Length marginAfter = style()->marginAfter();
    int& marginBeforeAlias = isHorizontal ? (isFlipped ? m_marginBottom : m_marginTop) : (isFlipped ? m_marginRight: m_marginLeft);
    int& marginAfterAlias = isHorizontal ? (isFlipped ? m_marginTop : m_marginBottom) : (isFlipped ? m_marginLeft: m_marginRight);

    Length logicalTop = style()->logicalTop();
    Length logicalBottom = style()->logicalBottom();
        
    /*---------------------------------------------------------------------------*\
     * For the purposes of this section and the next, the term "static position"
     * (of an element) refers, roughly, to the position an element would have had
     * in the normal flow. More precisely, the static position for 'top' is the
     * distance from the top edge of the containing block to the top margin edge
     * of a hypothetical box that would have been the first box of the element if
     * its 'position' property had been 'static' and 'float' had been 'none'. The
     * value is negative if the hypothetical box is above the containing block.
     *
     * But rather than actually calculating the dimensions of that hypothetical
     * box, user agents are free to make a guess at its probable position.
     *
     * For the purposes of calculating the static position, the containing block
     * of fixed positioned elements is the initial containing block instead of
     * the viewport.
    \*---------------------------------------------------------------------------*/

    computeBlockStaticDistance(logicalTop, logicalBottom, this, containerBlock);

    int logicalHeightResult; // Needed to compute overflow.
    int logicalTopPos;

    computePositionedLogicalHeightUsing(style()->logicalHeight(), containerBlock, containerLogicalHeight, bordersPlusPadding,
                                        logicalTop, logicalBottom, marginBefore, marginAfter,
                                        logicalHeightResult, marginBeforeAlias, marginAfterAlias, logicalTopPos);
    setLogicalTop(logicalTopPos);


    if (!style()->logicalMaxHeight().isUndefined()) {
        int maxLogicalHeight;
        int maxMarginBefore;
        int maxMarginAfter;
        int maxLogicalTopPos;

        computePositionedLogicalHeightUsing(style()->logicalMaxHeight(), containerBlock, containerLogicalHeight, bordersPlusPadding,
                                            logicalTop, logicalBottom, marginBefore, marginAfter,
                                            maxLogicalHeight, maxMarginBefore, maxMarginAfter, maxLogicalTopPos);

        if (logicalHeightResult > maxLogicalHeight) {
            logicalHeightResult = maxLogicalHeight;
            marginBeforeAlias = maxMarginBefore;
            marginAfterAlias = maxMarginAfter;
            setLogicalTop(maxLogicalTopPos);
        }
    }

    if (!style()->logicalMinHeight().isZero()) {
        int minLogicalHeight;
        int minMarginBefore;
        int minMarginAfter;
        int minLogicalTopPos;

        computePositionedLogicalHeightUsing(style()->logicalMinHeight(), containerBlock, containerLogicalHeight, bordersPlusPadding,
                                            logicalTop, logicalBottom, marginBefore, marginAfter,
                                            minLogicalHeight, minMarginBefore, minMarginAfter, minLogicalTopPos);

        if (logicalHeightResult < minLogicalHeight) {
            logicalHeightResult = minLogicalHeight;
            marginBeforeAlias = minMarginBefore;
            marginAfterAlias = minMarginAfter;
            setLogicalTop(minLogicalTopPos);
        }
    }

    setLogicalHeight(logicalHeightResult + bordersPlusPadding);
}
