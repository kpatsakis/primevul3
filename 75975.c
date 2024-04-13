LayoutUnit RenderBox::computeReplacedLogicalHeightUsing(Length logicalHeight) const
{
    switch (logicalHeight.type()) {
        case Fixed:
            return computeContentBoxLogicalHeight(logicalHeight.value());
        case Percent:
        {
            RenderObject* cb = isPositioned() ? container() : containingBlock();
            while (cb->isAnonymous()) {
                cb = cb->containingBlock();
                toRenderBlock(cb)->addPercentHeightDescendant(const_cast<RenderBox*>(this));
            }

            if (cb->isPositioned() && cb->style()->height().isAuto() && !(cb->style()->top().isAuto() || cb->style()->bottom().isAuto())) {
                ASSERT(cb->isRenderBlock());
                RenderBlock* block = toRenderBlock(cb);
                LayoutUnit oldHeight = block->height();
                block->computeLogicalHeight();
                LayoutUnit newHeight = block->computeContentBoxLogicalHeight(block->contentHeight());
                block->setHeight(oldHeight);
                return computeContentBoxLogicalHeight(logicalHeight.calcValue(newHeight));
            }
            
            LayoutUnit availableHeight;
            if (isPositioned())
                availableHeight = containingBlockLogicalHeightForPositioned(toRenderBoxModelObject(cb));
            else {
                availableHeight =  toRenderBox(cb)->availableLogicalHeight();
                while (cb && !cb->isRenderView() && (cb->style()->logicalHeight().isAuto() || cb->style()->logicalHeight().isPercent())) {
                    if (cb->isTableCell()) {
                        availableHeight = max(availableHeight, intrinsicLogicalHeight());
                        return logicalHeight.calcValue(availableHeight - borderAndPaddingLogicalHeight());
                    }
                    cb = cb->containingBlock();
                }
            }
            return computeContentBoxLogicalHeight(logicalHeight.calcValue(availableHeight));
        }
        default:
            return intrinsicLogicalHeight();
    }
}
