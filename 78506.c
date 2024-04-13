void RenderBlock::computeInlineDirectionPositionsForLine(RootInlineBox* lineBox, const LineInfo& lineInfo, BidiRun* firstRun, BidiRun* trailingSpaceRun, bool reachedEnd,
                                                         GlyphOverflowAndFallbackFontsMap& textBoxDataMap, VerticalPositionCache& verticalPositionCache, WordMeasurements& wordMeasurements)
{
    ETextAlign textAlign = textAlignmentForLine(!reachedEnd && !lineBox->endsWithBreak());

    bool isFirstLine = lineInfo.isFirstLine() && !(isAnonymousBlock() && parent()->firstChild() != this);
    bool isAfterHardLineBreak = lineBox->prevRootBox() && lineBox->prevRootBox()->endsWithBreak();
    IndentTextOrNot shouldIndentText = requiresIndent(isFirstLine, isAfterHardLineBreak, style());
    float lineLogicalLeft;
    float lineLogicalRight;
    float availableLogicalWidth;
    updateLogicalInlinePositions(this, lineLogicalLeft, lineLogicalRight, availableLogicalWidth, isFirstLine, shouldIndentText, 0);
    bool needsWordSpacing;
    ShapeInsideInfo* shapeInsideInfo = layoutShapeInsideInfo();
    if (shapeInsideInfo && shapeInsideInfo->hasSegments()) {
        BidiRun* segmentStart = firstRun;
        const SegmentList& segments = shapeInsideInfo->segments();
        float logicalLeft = max<float>(roundToInt(segments[0].logicalLeft), lineLogicalLeft);
        float logicalRight = min<float>(floorToInt(segments[0].logicalRight), lineLogicalRight);
        float startLogicalLeft = logicalLeft;
        float endLogicalRight = logicalLeft;
        float minLogicalLeft = logicalLeft;
        float maxLogicalRight = logicalLeft;
        lineBox->beginPlacingBoxRangesInInlineDirection(logicalLeft);
        for (size_t i = 0; i < segments.size(); i++) {
            if (i) {
                logicalLeft = max<float>(roundToInt(segments[i].logicalLeft), lineLogicalLeft);
                logicalRight = min<float>(floorToInt(segments[i].logicalRight), lineLogicalRight);
            }
            availableLogicalWidth = logicalRight - logicalLeft;
            BidiRun* newSegmentStart = computeInlineDirectionPositionsForSegment(lineBox, lineInfo, textAlign, logicalLeft, availableLogicalWidth, segmentStart, trailingSpaceRun, textBoxDataMap, verticalPositionCache, wordMeasurements);
            needsWordSpacing = false;
            endLogicalRight = lineBox->placeBoxRangeInInlineDirection(segmentStart->m_box, newSegmentStart ? newSegmentStart->m_box : 0, logicalLeft, minLogicalLeft, maxLogicalRight, needsWordSpacing, textBoxDataMap);
            if (!newSegmentStart || !newSegmentStart->next())
                break;
            ASSERT(newSegmentStart->m_startsSegment);
            segmentStart = newSegmentStart->next();
        }
        lineBox->endPlacingBoxRangesInInlineDirection(startLogicalLeft, endLogicalRight, minLogicalLeft, maxLogicalRight);
        return;
    }

    if (firstRun && firstRun->m_object->isReplaced()) {
        RenderBox* renderBox = toRenderBox(firstRun->m_object);
        updateLogicalInlinePositions(this, lineLogicalLeft, lineLogicalRight, availableLogicalWidth, isFirstLine, shouldIndentText, renderBox->logicalHeight());
    }

    computeInlineDirectionPositionsForSegment(lineBox, lineInfo, textAlign, lineLogicalLeft, availableLogicalWidth, firstRun, trailingSpaceRun, textBoxDataMap, verticalPositionCache, wordMeasurements);
    needsWordSpacing = false;
    lineBox->placeBoxesInInlineDirection(lineLogicalLeft, needsWordSpacing, textBoxDataMap);
}
