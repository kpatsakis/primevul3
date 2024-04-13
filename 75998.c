bool RenderBox::logicalScroll(ScrollLogicalDirection direction, ScrollGranularity granularity, float multiplier, Node** stopNode)
{
    bool scrolled = false;
    
    RenderLayer* l = layer();
    if (l) {
#if PLATFORM(MAC)
        if (granularity == ScrollByDocument)
            scrolled = l->scroll(logicalToPhysical(ScrollInlineDirectionBackward, isHorizontalWritingMode(), style()->isFlippedBlocksWritingMode()), ScrollByDocument, multiplier);
#endif
        if (l->scroll(logicalToPhysical(direction, isHorizontalWritingMode(), style()->isFlippedBlocksWritingMode()), granularity, multiplier))
            scrolled = true;
        
        if (scrolled) {
            if (stopNode)
                *stopNode = node();
            return true;
        }
    }

    if (stopNode && *stopNode && *stopNode == node())
        return true;

    RenderBlock* b = containingBlock();
    if (b && !b->isRenderView())
        return b->logicalScroll(direction, granularity, multiplier, stopNode);
    return false;
}
