void RenderBox::computePositionedLogicalHeightReplaced()
{

    const RenderBoxModelObject* containerBlock = toRenderBoxModelObject(container());

    const int containerLogicalHeight = containingBlockLogicalHeightForPositioned(containerBlock);

    bool isHorizontal = isHorizontalWritingMode();
    bool isFlipped = style()->isFlippedBlocksWritingMode();
    Length marginBefore = style()->marginBefore();
    Length marginAfter = style()->marginAfter();
    int& marginBeforeAlias = isHorizontal ? (isFlipped ? m_marginBottom : m_marginTop) : (isFlipped ? m_marginRight: m_marginLeft);
    int& marginAfterAlias = isHorizontal ? (isFlipped ? m_marginTop : m_marginBottom) : (isFlipped ? m_marginLeft: m_marginRight);

    Length logicalTop = style()->logicalTop();
    Length logicalBottom = style()->logicalBottom();

    /*-----------------------------------------------------------------------*\
     * 1. The used value of 'height' is determined as for inline replaced
     *    elements.
    \*-----------------------------------------------------------------------*/
    setLogicalHeight(computeReplacedLogicalHeight() + borderAndPaddingLogicalHeight());
    const int availableSpace = containerLogicalHeight - logicalHeight();

    /*-----------------------------------------------------------------------*\
     * 2. If both 'top' and 'bottom' have the value 'auto', replace 'top'
     *    with the element's static position.
    \*-----------------------------------------------------------------------*/
    computeBlockStaticDistance(logicalTop, logicalBottom, this, containerBlock);

    /*-----------------------------------------------------------------------*\
     * 3. If 'bottom' is 'auto', replace any 'auto' on 'margin-top' or
     *    'margin-bottom' with '0'.
    \*-----------------------------------------------------------------------*/
    if (logicalTop.isAuto() || logicalBottom.isAuto()) {
        if (marginBefore.isAuto())
            marginBefore.setValue(Fixed, 0);
        if (marginAfter.isAuto())
            marginAfter.setValue(Fixed, 0);
    }

    /*-----------------------------------------------------------------------*\
     * 4. If at this point both 'margin-top' and 'margin-bottom' are still
     *    'auto', solve the equation under the extra constraint that the two
     *    margins must get equal values.
    \*-----------------------------------------------------------------------*/
    int logicalTopValue = 0;
    int logicalBottomValue = 0;

    if (marginBefore.isAuto() && marginAfter.isAuto()) {
        ASSERT(!(logicalTop.isAuto() || logicalBottom.isAuto()));

        logicalTopValue = logicalTop.calcValue(containerLogicalHeight);
        logicalBottomValue = logicalBottom.calcValue(containerLogicalHeight);

        int difference = availableSpace - (logicalTopValue + logicalBottomValue);
        marginBeforeAlias =  difference / 2; // split the difference
        marginAfterAlias = difference - marginBeforeAlias; // account for odd valued differences

    /*-----------------------------------------------------------------------*\
     * 5. If at this point there is only one 'auto' left, solve the equation
     *    for that value.
    \*-----------------------------------------------------------------------*/
    } else if (logicalTop.isAuto()) {
        marginBeforeAlias = marginBefore.calcValue(containerLogicalHeight);
        marginAfterAlias = marginAfter.calcValue(containerLogicalHeight);
        logicalBottomValue = logicalBottom.calcValue(containerLogicalHeight);

        logicalTopValue = availableSpace - (logicalBottomValue + marginBeforeAlias + marginAfterAlias);
    } else if (logicalBottom.isAuto()) {
        marginBeforeAlias = marginBefore.calcValue(containerLogicalHeight);
        marginAfterAlias = marginAfter.calcValue(containerLogicalHeight);
        logicalTopValue = logicalTop.calcValue(containerLogicalHeight);

    } else if (marginBefore.isAuto()) {
        marginAfterAlias = marginAfter.calcValue(containerLogicalHeight);
        logicalTopValue = logicalTop.calcValue(containerLogicalHeight);
        logicalBottomValue = logicalBottom.calcValue(containerLogicalHeight);

        marginBeforeAlias = availableSpace - (logicalTopValue + logicalBottomValue + marginAfterAlias);
    } else if (marginAfter.isAuto()) {
        marginBeforeAlias = marginBefore.calcValue(containerLogicalHeight);
        logicalTopValue = logicalTop.calcValue(containerLogicalHeight);
        logicalBottomValue = logicalBottom.calcValue(containerLogicalHeight);

        marginAfterAlias = availableSpace - (logicalTopValue + logicalBottomValue + marginBeforeAlias);
    } else {
        marginBeforeAlias = marginBefore.calcValue(containerLogicalHeight);
        marginAfterAlias = marginAfter.calcValue(containerLogicalHeight);
        logicalTopValue = logicalTop.calcValue(containerLogicalHeight);
     }

    /*-----------------------------------------------------------------------*\
     * 6. If at this point the values are over-constrained, ignore the value
     *    for 'bottom' and solve for that value.
    \*-----------------------------------------------------------------------*/

    int logicalTopPos = logicalTopValue + marginBeforeAlias;
    computeLogicalTopPositionedOffset(logicalTopPos, this, logicalHeight(), containerBlock, containerLogicalHeight);
    setLogicalTop(logicalTopPos);
}
