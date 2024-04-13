void RenderBox::computeLogicalHeight()
{
    if (isTableCell() || (isInline() && !isReplaced()))
        return;

    Length h;
    if (isPositioned()) {
        computePositionedLogicalHeight();
    } else {
        RenderBlock* cb = containingBlock();
        bool hasPerpendicularContainingBlock = cb->isHorizontalWritingMode() != isHorizontalWritingMode();
    
        if (!hasPerpendicularContainingBlock)
            computeBlockDirectionMargins(cb);

        if (isTable()) {
            if (hasPerpendicularContainingBlock)
                computeInlineDirectionMargins(cb, containingBlockLogicalWidthForContent(), logicalHeight());
            return;
        }

        bool inHorizontalBox = parent()->isDeprecatedFlexibleBox() && parent()->style()->boxOrient() == HORIZONTAL;
        bool stretching = parent()->style()->boxAlign() == BSTRETCH;
        bool treatAsReplaced = shouldComputeSizeAsReplaced() && (!inHorizontalBox || !stretching);
        bool checkMinMaxHeight = false;

        if (hasOverrideSize() && parent()->isDeprecatedFlexibleBox() && parent()->style()->boxOrient() == VERTICAL
                && parent()->isFlexingChildren())
            h = Length(overrideHeight() - borderAndPaddingLogicalHeight(), Fixed);
        else if (treatAsReplaced)
            h = Length(computeReplacedLogicalHeight(), Fixed);
        else {
            h = style()->logicalHeight();
            checkMinMaxHeight = true;
        }

        if (h.isAuto() && parent()->isDeprecatedFlexibleBox() && parent()->style()->boxOrient() == HORIZONTAL
                && parent()->isStretchingChildren()) {
            h = Length(parentBox()->contentLogicalHeight() - marginBefore() - marginAfter() - borderAndPaddingLogicalHeight(), Fixed);
            checkMinMaxHeight = false;
        }

        LayoutUnit heightResult;
        if (checkMinMaxHeight) {
            heightResult = computeLogicalHeightUsing(style()->logicalHeight());
            if (heightResult == -1)
                heightResult = logicalHeight();
            LayoutUnit minH = computeLogicalHeightUsing(style()->logicalMinHeight()); // Leave as -1 if unset.
            LayoutUnit maxH = style()->logicalMaxHeight().isUndefined() ? heightResult : computeLogicalHeightUsing(style()->logicalMaxHeight());
            if (maxH == -1)
                maxH = heightResult;
            heightResult = min(maxH, heightResult);
            heightResult = max(minH, heightResult);
        } else {
            heightResult = h.value() + borderAndPaddingLogicalHeight();
        }

        setLogicalHeight(heightResult);
        
        if (hasPerpendicularContainingBlock)
            computeInlineDirectionMargins(cb, containingBlockLogicalWidthForContent(), heightResult);
    }

    bool paginatedContentNeedsBaseHeight = document()->printing() && h.isPercent()
        && (isRoot() || (isBody() && document()->documentElement()->renderer()->style()->logicalHeight().isPercent()));
    if (stretchesToViewport() || paginatedContentNeedsBaseHeight) {
        LayoutUnit margins = collapsedMarginBefore() + collapsedMarginAfter();
        LayoutUnit visHeight;
        if (document()->printing())
            visHeight = static_cast<LayoutUnit>(view()->pageLogicalHeight());
        else  {
            if (isHorizontalWritingMode())
                visHeight = view()->viewHeight();
            else
                visHeight = view()->viewWidth();
        }
        if (isRoot())
            setLogicalHeight(max(logicalHeight(), visHeight - margins));
        else {
            LayoutUnit marginsBordersPadding = margins + parentBox()->marginBefore() + parentBox()->marginAfter() + parentBox()->borderAndPaddingLogicalHeight();
            setLogicalHeight(max(logicalHeight(), visHeight - marginsBordersPadding));
        }
    }
}
