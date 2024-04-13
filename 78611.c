void RenderBlock::updateShapeAndSegmentsForCurrentLineInFlowThread(ShapeInsideInfo*& shapeInsideInfo, LineLayoutState& layoutState)
{
    ASSERT(layoutState.flowThread());

    LayoutUnit lineHeight = this->lineHeight(layoutState.lineInfo().isFirstLine(), isHorizontalWritingMode() ? HorizontalLine : VerticalLine, PositionOfInteriorLineBoxes);

    RenderRegion* currentRegion = regionAtBlockOffset(logicalHeight());
    if (!currentRegion)
        return;

    shapeInsideInfo = currentRegion->shapeInsideInfo();

    LayoutUnit logicalLineTopInFlowThread = logicalHeight() + offsetFromLogicalTopOfFirstPage();
    LayoutUnit logicalLineBottomInFlowThread = logicalLineTopInFlowThread + lineHeight;
    LayoutUnit logicalRegionTopInFlowThread = currentRegion->logicalTopForFlowThreadContent();
    LayoutUnit logicalRegionBottomInFlowThread = logicalRegionTopInFlowThread + currentRegion->logicalHeight() - currentRegion->borderAndPaddingBefore() - currentRegion->borderAndPaddingAfter();

    if (!shapeInsideInfo && !currentRegion->isLastRegion()) {
        LayoutUnit deltaToNextRegion = logicalHeight() + logicalRegionBottomInFlowThread - logicalLineTopInFlowThread;
        RenderRegion* lookupForNextRegion = regionAtBlockOffset(logicalHeight() + deltaToNextRegion);
        if (!lookupForNextRegion->shapeInsideInfo())
            return;
    }

    LayoutUnit shapeBottomInFlowThread = LayoutUnit::max();
    if (shapeInsideInfo)
        shapeBottomInFlowThread = shapeInsideInfo->shapeLogicalBottom() + currentRegion->logicalTopForFlowThreadContent();

    RenderRegion* nextRegion = regionAtBlockOffset(logicalHeight() + lineHeight);
    if ((currentRegion != nextRegion && (logicalLineBottomInFlowThread > logicalRegionBottomInFlowThread)) || (!currentRegion->isLastRegion() && shapeBottomInFlowThread < logicalLineBottomInFlowThread)) {
        LayoutUnit deltaToNextRegion = logicalRegionBottomInFlowThread - logicalLineTopInFlowThread;
        nextRegion = regionAtBlockOffset(logicalHeight() + deltaToNextRegion);

        ASSERT(currentRegion != nextRegion);

        shapeInsideInfo = nextRegion->shapeInsideInfo();
        setLogicalHeight(logicalHeight() + deltaToNextRegion);

        currentRegion = nextRegion;

        logicalLineTopInFlowThread = logicalHeight() + offsetFromLogicalTopOfFirstPage();
        logicalLineBottomInFlowThread = logicalLineTopInFlowThread + lineHeight;
        logicalRegionTopInFlowThread = currentRegion->logicalTopForFlowThreadContent();
        logicalRegionBottomInFlowThread = logicalRegionTopInFlowThread + currentRegion->logicalHeight() - currentRegion->borderAndPaddingBefore() - currentRegion->borderAndPaddingAfter();
    }

    if (!shapeInsideInfo)
        return;

    if (logicalLineBottomInFlowThread <= (logicalRegionTopInFlowThread + lineHeight) || (logicalLineTopInFlowThread - logicalRegionTopInFlowThread) < (layoutState.adjustedLogicalLineTop() - currentRegion->borderAndPaddingBefore())) {
        LayoutUnit shapeTopOffset = layoutState.adjustedLogicalLineTop();
        if (!shapeTopOffset)
            shapeTopOffset = shapeInsideInfo->shapeLogicalTop();

        LayoutUnit shapePositionInFlowThread = currentRegion->logicalTopForFlowThreadContent() + shapeTopOffset;
        LayoutUnit shapeTopLineTopDelta = shapePositionInFlowThread - logicalLineTopInFlowThread - currentRegion->borderAndPaddingBefore();

        setLogicalHeight(logicalHeight() + shapeTopLineTopDelta);
        logicalLineTopInFlowThread += shapeTopLineTopDelta;
        layoutState.setAdjustedLogicalLineTop(0);
    }

    LayoutUnit lineTop = logicalLineTopInFlowThread - currentRegion->logicalTopForFlowThreadContent() + currentRegion->borderAndPaddingBefore();
    shapeInsideInfo->computeSegmentsForLine(lineTop, lineHeight);

    if (currentRegion->isLastRegion())
        pushShapeContentOverflowBelowTheContentBox(this, shapeInsideInfo, lineTop, lineHeight);
}
