static inline void constructBidiRunsForLine(const RenderBlock* block, InlineBidiResolver& topResolver, BidiRunList<BidiRun>& bidiRuns, const InlineIterator& endOfLine, VisualDirectionOverride override, bool previousLineBrokeCleanly)
{
    ShapeInsideInfo* shapeInsideInfo = block->layoutShapeInsideInfo();
    if (!shapeInsideInfo || !shapeInsideInfo->hasSegments()) {
        constructBidiRunsForSegment(topResolver, bidiRuns, endOfLine, override, previousLineBrokeCleanly);
        return;
    }

    const SegmentRangeList& segmentRanges = shapeInsideInfo->segmentRanges();
    ASSERT(segmentRanges.size());

    for (size_t i = 0; i < segmentRanges.size(); i++) {
        LineSegmentIterator iterator = segmentRanges[i].start;
        InlineIterator segmentStart(iterator.root, iterator.object, iterator.offset);
        iterator = segmentRanges[i].end;
        InlineIterator segmentEnd(iterator.root, iterator.object, iterator.offset);
        if (i) {
            ASSERT(segmentStart.m_obj);
            BidiRun* segmentMarker = createRun(segmentStart.m_pos, segmentStart.m_pos, segmentStart.m_obj, topResolver);
            segmentMarker->m_startsSegment = true;
            bidiRuns.addRun(segmentMarker);
            topResolver.midpointState().betweenMidpoints = false;
        }
        if (!segmentIsEmpty(segmentStart, segmentEnd)) {
            topResolver.setPosition(segmentStart, numberOfIsolateAncestors(segmentStart));
            constructBidiRunsForSegment(topResolver, bidiRuns, segmentEnd, override, previousLineBrokeCleanly);
        }
    }
}
