void RenderBox::computePositionedLogicalWidthReplaced()
{

    const RenderBoxModelObject* containerBlock = toRenderBoxModelObject(container());

    const int containerLogicalWidth = containingBlockLogicalWidthForPositioned(containerBlock);

    TextDirection containerDirection = (document()->inQuirksMode()) ? parent()->style()->direction() : containerBlock->style()->direction();

    bool isHorizontal = isHorizontalWritingMode();
    Length logicalLeft = style()->logicalLeft();
    Length logicalRight = style()->logicalRight();
    Length marginLogicalLeft = isHorizontal ? style()->marginLeft() : style()->marginTop();
    Length marginLogicalRight = isHorizontal ? style()->marginRight() : style()->marginBottom();
    int& marginLogicalLeftAlias = isHorizontal ? m_marginLeft : m_marginTop;
    int& marginLogicalRightAlias = isHorizontal ? m_marginRight : m_marginBottom;

    /*-----------------------------------------------------------------------*\
     * 1. The used value of 'width' is determined as for inline replaced
     *    elements.
    \*-----------------------------------------------------------------------*/
    setLogicalWidth(computeReplacedLogicalWidth() + borderAndPaddingLogicalWidth());
    const int availableSpace = containerLogicalWidth - logicalWidth();

    /*-----------------------------------------------------------------------*\
     * 2. If both 'left' and 'right' have the value 'auto', then if 'direction'
     *    of the containing block is 'ltr', set 'left' to the static position;
     *    else if 'direction' is 'rtl', set 'right' to the static position.
    \*-----------------------------------------------------------------------*/
    computeInlineStaticDistance(logicalLeft, logicalRight, this, containerBlock, containerLogicalWidth, containerDirection);

    /*-----------------------------------------------------------------------*\
     * 3. If 'left' or 'right' are 'auto', replace any 'auto' on 'margin-left'
     *    or 'margin-right' with '0'.
    \*-----------------------------------------------------------------------*/
    if (logicalLeft.isAuto() || logicalRight.isAuto()) {
        if (marginLogicalLeft.isAuto())
            marginLogicalLeft.setValue(Fixed, 0);
        if (marginLogicalRight.isAuto())
            marginLogicalRight.setValue(Fixed, 0);
    }

    /*-----------------------------------------------------------------------*\
     * 4. If at this point both 'margin-left' and 'margin-right' are still
     *    'auto', solve the equation under the extra constraint that the two
     *    margins must get equal values, unless this would make them negative,
     *    in which case when the direction of the containing block is 'ltr'
     *    ('rtl'), set 'margin-left' ('margin-right') to zero and solve for
     *    'margin-right' ('margin-left').
    \*-----------------------------------------------------------------------*/
    int logicalLeftValue = 0;
    int logicalRightValue = 0;

    if (marginLogicalLeft.isAuto() && marginLogicalRight.isAuto()) {
        ASSERT(!(logicalLeft.isAuto() && logicalRight.isAuto()));

        logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);
        logicalRightValue = logicalRight.calcValue(containerLogicalWidth);

        int difference = availableSpace - (logicalLeftValue + logicalRightValue);
        if (difference > 0) {
            marginLogicalLeftAlias = difference / 2; // split the difference
            marginLogicalRightAlias = difference - marginLogicalLeftAlias; // account for odd valued differences
        } else {
            if (containerDirection == LTR) {
                marginLogicalLeftAlias = 0;
                marginLogicalRightAlias = difference; // will be negative
            } else {
                marginLogicalLeftAlias = difference; // will be negative
                marginLogicalRightAlias = 0;
            }
        }

    /*-----------------------------------------------------------------------*\
     * 5. If at this point there is an 'auto' left, solve the equation for
     *    that value.
    \*-----------------------------------------------------------------------*/
    } else if (logicalLeft.isAuto()) {
        marginLogicalLeftAlias = marginLogicalLeft.calcValue(containerLogicalWidth);
        marginLogicalRightAlias = marginLogicalRight.calcValue(containerLogicalWidth);
        logicalRightValue = logicalRight.calcValue(containerLogicalWidth);

        logicalLeftValue = availableSpace - (logicalRightValue + marginLogicalLeftAlias + marginLogicalRightAlias);
    } else if (logicalRight.isAuto()) {
        marginLogicalLeftAlias = marginLogicalLeft.calcValue(containerLogicalWidth);
        marginLogicalRightAlias = marginLogicalRight.calcValue(containerLogicalWidth);
        logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);

        logicalRightValue = availableSpace - (logicalLeftValue + marginLogicalLeftAlias + marginLogicalRightAlias);
    } else if (marginLogicalLeft.isAuto()) {
        marginLogicalRightAlias = marginLogicalRight.calcValue(containerLogicalWidth);
        logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);
        logicalRightValue = logicalRight.calcValue(containerLogicalWidth);

        marginLogicalLeftAlias = availableSpace - (logicalLeftValue + logicalRightValue + marginLogicalRightAlias);
    } else if (marginLogicalRight.isAuto()) {
        marginLogicalLeftAlias = marginLogicalLeft.calcValue(containerLogicalWidth);
        logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);
        logicalRightValue = logicalRight.calcValue(containerLogicalWidth);

        marginLogicalRightAlias = availableSpace - (logicalLeftValue + logicalRightValue + marginLogicalLeftAlias);
    } else {
        marginLogicalLeftAlias = marginLogicalLeft.calcValue(containerLogicalWidth);
        marginLogicalRightAlias = marginLogicalRight.calcValue(containerLogicalWidth);
        logicalRightValue = logicalRight.calcValue(containerLogicalWidth);
        logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);
    }

    /*-----------------------------------------------------------------------*\
     * 6. If at this point the values are over-constrained, ignore the value
     *    for either 'left' (in case the 'direction' property of the
     *    containing block is 'rtl') or 'right' (in case 'direction' is
     *    'ltr') and solve for that value.
    \*-----------------------------------------------------------------------*/
    int totalLogicalWidth = logicalWidth() + logicalLeftValue + logicalRightValue +  marginLogicalLeftAlias + marginLogicalRightAlias;
    if (totalLogicalWidth > containerLogicalWidth && (containerDirection == RTL))
        logicalLeftValue = containerLogicalWidth - (totalLogicalWidth - logicalLeftValue);

    

    if (containerBlock->isRenderInline() && !containerBlock->style()->isLeftToRightDirection()) {
        const RenderInline* flow = toRenderInline(containerBlock);
        InlineFlowBox* firstLine = flow->firstLineBox();
        InlineFlowBox* lastLine = flow->lastLineBox();
        if (firstLine && lastLine && firstLine != lastLine) {
            setLogicalLeft(logicalLeftValue + marginLogicalLeftAlias + lastLine->borderLogicalLeft() + (lastLine->logicalLeft() - firstLine->logicalLeft()));
            return;
        }
    }

    int logicalLeftPos = logicalLeftValue + marginLogicalLeftAlias;
    computeLogicalLeftPositionedOffset(logicalLeftPos, this, logicalWidth(), containerBlock, containerLogicalWidth);    
    setLogicalLeft(logicalLeftPos);
}
