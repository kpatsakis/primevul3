LayoutUnit RenderBlock::startAlignedOffsetForLine(LayoutUnit position, bool firstLine)
{
    ETextAlign textAlign = style()->textAlign();

    if (textAlign == TASTART) // FIXME: Handle TAEND here
        return startOffsetForLine(position, firstLine);

    float totalLogicalWidth = 0;
    float logicalLeft = logicalLeftOffsetForLine(logicalHeight(), false);
    float availableLogicalWidth = logicalRightOffsetForLine(logicalHeight(), false) - logicalLeft;
    updateLogicalWidthForAlignment(textAlign, 0, 0, logicalLeft, totalLogicalWidth, availableLogicalWidth, 0);

    if (!style()->isLeftToRightDirection())
        return logicalWidth() - logicalLeft;
    return logicalLeft;
}
