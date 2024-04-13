static void setStaticPositions(RenderBlock* block, RenderBox* child)
{
    RenderObject* containerBlock = child->container();
    LayoutUnit blockHeight = block->logicalHeight();
    if (containerBlock->isRenderInline()) {
        toRenderInline(containerBlock)->layer()->setStaticInlinePosition(block->startAlignedOffsetForLine(blockHeight, false));
        toRenderInline(containerBlock)->layer()->setStaticBlockPosition(blockHeight);
    }
    block->updateStaticInlinePositionForChild(child, blockHeight);
    child->layer()->setStaticBlockPosition(blockHeight);
}
