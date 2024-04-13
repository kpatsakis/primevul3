void RenderBlock::updateShapeAndSegmentsForCurrentLine(ShapeInsideInfo*& shapeInsideInfo, LayoutUnit& absoluteLogicalTop, LineLayoutState& layoutState)
{
    if (layoutState.flowThread())
        return updateShapeAndSegmentsForCurrentLineInFlowThread(shapeInsideInfo, layoutState);

    if (!shapeInsideInfo)
        return;

    LayoutUnit lineTop = logicalHeight() + absoluteLogicalTop;
    LayoutUnit lineHeight = this->lineHeight(layoutState.lineInfo().isFirstLine(), isHorizontalWritingMode() ? HorizontalLine : VerticalLine, PositionOfInteriorLineBoxes);

    shapeInsideInfo->computeSegmentsForLine(lineTop, lineHeight);

    pushShapeContentOverflowBelowTheContentBox(this, shapeInsideInfo, lineTop, lineHeight);
}
