static void updateLogicalInlinePositions(RenderBlock* block, float& lineLogicalLeft, float& lineLogicalRight, float& availableLogicalWidth, bool firstLine, IndentTextOrNot shouldIndentText, LayoutUnit boxLogicalHeight)
{
    LayoutUnit lineLogicalHeight = logicalHeightForLine(block, firstLine, boxLogicalHeight);
    lineLogicalLeft = block->pixelSnappedLogicalLeftOffsetForLine(block->logicalHeight(), shouldIndentText == IndentText, lineLogicalHeight);
    lineLogicalRight = block->pixelSnappedLogicalRightOffsetForLine(block->logicalHeight(), shouldIndentText == IndentText, lineLogicalHeight);
    availableLogicalWidth = lineLogicalRight - lineLogicalLeft;
}
