LayoutUnit RenderBox::computePercentageLogicalHeight(const Length& height)
{
    LayoutUnit result = -1;
    
    bool skippedAutoHeightContainingBlock = false;
    RenderBlock* cb = containingBlock();
    while (!cb->isRenderView() && !cb->isBody() && !cb->isTableCell() && !cb->isPositioned() && cb->style()->logicalHeight().isAuto()) {
        if (!document()->inQuirksMode() && !cb->isAnonymousBlock())
            break;
        skippedAutoHeightContainingBlock = true;
        cb = cb->containingBlock();
        cb->addPercentHeightDescendant(this);
    }

    bool isPositionedWithSpecifiedHeight = cb->isPositioned() && (!cb->style()->logicalHeight().isAuto() || (!cb->style()->top().isAuto() && !cb->style()->bottom().isAuto()));

    bool includeBorderPadding = isTable();

    if (cb->isTableCell()) {
        if (!skippedAutoHeightContainingBlock) {
            if (!cb->hasOverrideSize()) {
                RenderTableCell* cell = toRenderTableCell(cb);
                if (scrollsOverflowY() && (!cell->style()->logicalHeight().isAuto() || !cell->table()->style()->logicalHeight().isAuto()))
                    return 0;
                return -1;
            }
            result = cb->overrideHeight();
            includeBorderPadding = true;
        }
    }
    else if (cb->style()->logicalHeight().isFixed())
        result = cb->computeContentBoxLogicalHeight(cb->style()->logicalHeight().value());
    else if (cb->style()->logicalHeight().isPercent() && !isPositionedWithSpecifiedHeight) {
        result = cb->computePercentageLogicalHeight(cb->style()->logicalHeight());
        if (result != -1)
            result = cb->computeContentBoxLogicalHeight(result);
    } else if (cb->isRenderView() || (cb->isBody() && document()->inQuirksMode()) || isPositionedWithSpecifiedHeight) {
        LayoutUnit oldHeight = cb->logicalHeight();
        cb->computeLogicalHeight();
        result = cb->contentLogicalHeight();
        cb->setLogicalHeight(oldHeight);
    } else if (cb->isRoot() && isPositioned())
        result = cb->computeContentBoxLogicalHeight(cb->availableLogicalHeight());

    if (result != -1) {
        result = height.calcValue(result);
        if (includeBorderPadding) {
            result -= borderAndPaddingLogicalHeight();
            result = max<LayoutUnit>(0, result);
        }
    }
    return result;
}
