bool RenderBlock::adjustLogicalLineTopAndLogicalHeightIfNeeded(ShapeInsideInfo* shapeInsideInfo, LayoutUnit absoluteLogicalTop, LineLayoutState& layoutState, InlineBidiResolver& resolver, FloatingObject* lastFloatFromPreviousLine, InlineIterator& end, WordMeasurements& wordMeasurements)
{
    LayoutUnit adjustedLogicalLineTop = adjustLogicalLineTop(shapeInsideInfo, resolver.position(), end, wordMeasurements);
    if (!adjustedLogicalLineTop)
        return false;

    LayoutUnit newLogicalHeight = adjustedLogicalLineTop - absoluteLogicalTop;

    if (layoutState.flowThread()) {
        layoutState.setAdjustedLogicalLineTop(adjustedLogicalLineTop);
        newLogicalHeight = logicalHeight();
    }


    end = restartLayoutRunsAndFloatsInRange(logicalHeight(), newLogicalHeight, lastFloatFromPreviousLine, resolver, end);
    return true;
}
