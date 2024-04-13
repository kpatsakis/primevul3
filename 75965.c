void RenderBox::computeLogicalWidth()
{
    if (isPositioned()) {
        computePositionedLogicalWidth();
        return;
    }

    if (node() && view()->frameView() && view()->frameView()->layoutRoot(true) == this)
        return;

    if (hasOverrideSize() &&  parent()->style()->boxOrient() == HORIZONTAL
            && parent()->isDeprecatedFlexibleBox() && parent()->isFlexingChildren()) {
        setLogicalWidth(overrideWidth());
        return;
    }

    bool inVerticalBox = parent()->isDeprecatedFlexibleBox() && (parent()->style()->boxOrient() == VERTICAL);
    bool stretching = (parent()->style()->boxAlign() == BSTRETCH);
    bool treatAsReplaced = shouldComputeSizeAsReplaced() && (!inVerticalBox || !stretching);

    Length logicalWidthLength = (treatAsReplaced) ? Length(computeReplacedLogicalWidth(), Fixed) : style()->logicalWidth();

    RenderBlock* cb = containingBlock();
    int containerLogicalWidth = max(0, containingBlockLogicalWidthForContent());
    bool hasPerpendicularContainingBlock = cb->isHorizontalWritingMode() != isHorizontalWritingMode();
    int containerWidthInInlineDirection = containerLogicalWidth;
    if (hasPerpendicularContainingBlock)
        containerWidthInInlineDirection = perpendicularContainingBlockLogicalHeight();
    
    if (isInline() && !isInlineBlockOrInlineTable()) {
        setMarginStart(style()->marginStart().calcMinValue(containerLogicalWidth));
        setMarginEnd(style()->marginEnd().calcMinValue(containerLogicalWidth));
        if (treatAsReplaced)
            setLogicalWidth(max(logicalWidthLength.value() + borderAndPaddingLogicalWidth(), minPreferredLogicalWidth()));
        return;
    }

    if (treatAsReplaced)
        setLogicalWidth(logicalWidthLength.value() + borderAndPaddingLogicalWidth());
    else {
        setLogicalWidth(computeLogicalWidthUsing(LogicalWidth, containerWidthInInlineDirection));

        if (!style()->logicalMaxWidth().isUndefined()) {
            int maxLogicalWidth = computeLogicalWidthUsing(MaxLogicalWidth, containerWidthInInlineDirection);
            if (logicalWidth() > maxLogicalWidth) {
                setLogicalWidth(maxLogicalWidth);
                logicalWidthLength = style()->logicalMaxWidth();
            }
        }

        int minLogicalWidth = computeLogicalWidthUsing(MinLogicalWidth, containerWidthInInlineDirection);
        if (logicalWidth() < minLogicalWidth) {
            setLogicalWidth(minLogicalWidth);
            logicalWidthLength = style()->logicalMinWidth();
        }
    }

    if (stretchesToMinIntrinsicLogicalWidth()) {
        setLogicalWidth(max(logicalWidth(), minPreferredLogicalWidth()));
        logicalWidthLength = Length(logicalWidth(), Fixed);
    }

    if (logicalWidthLength.isAuto() || hasPerpendicularContainingBlock) {
        setMarginStart(style()->marginStart().calcMinValue(containerLogicalWidth));
        setMarginEnd(style()->marginEnd().calcMinValue(containerLogicalWidth));
    } else
        computeInlineDirectionMargins(cb, containerLogicalWidth, logicalWidth());

    if (!hasPerpendicularContainingBlock && containerLogicalWidth && containerLogicalWidth != (logicalWidth() + marginStart() + marginEnd())
            && !isFloating() && !isInline() && !cb->isDeprecatedFlexibleBox())
        cb->setMarginEndForChild(this, containerLogicalWidth - logicalWidth() - cb->marginStartForChild(this));
}
