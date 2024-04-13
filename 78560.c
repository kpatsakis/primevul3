InlineIterator RenderBlock::LineBreaker::nextLineBreak(InlineBidiResolver& resolver, LineInfo& lineInfo, RenderTextInfo& renderTextInfo, FloatingObject* lastFloatFromPreviousLine, unsigned consecutiveHyphenatedLines, WordMeasurements& wordMeasurements)
{
    ShapeInsideInfo* shapeInsideInfo = m_block->layoutShapeInsideInfo();

    if (!shapeInsideInfo || !shapeInsideInfo->lineOverlapsShapeBounds())
        return nextSegmentBreak(resolver, lineInfo, renderTextInfo, lastFloatFromPreviousLine, consecutiveHyphenatedLines, wordMeasurements);

    InlineIterator end = resolver.position();
    InlineIterator oldEnd = end;

    if (!shapeInsideInfo->hasSegments()) {
        end = nextSegmentBreak(resolver, lineInfo, renderTextInfo, lastFloatFromPreviousLine, consecutiveHyphenatedLines, wordMeasurements);
        resolver.setPositionIgnoringNestedIsolates(oldEnd);
        return oldEnd;
    }

    const SegmentList& segments = shapeInsideInfo->segments();
    SegmentRangeList& segmentRanges = shapeInsideInfo->segmentRanges();

    for (unsigned i = 0; i < segments.size() && !end.atEnd(); i++) {
        InlineIterator segmentStart = resolver.position();
        end = nextSegmentBreak(resolver, lineInfo, renderTextInfo, lastFloatFromPreviousLine, consecutiveHyphenatedLines, wordMeasurements);

        ASSERT(segmentRanges.size() == i);
        if (resolver.position().atEnd()) {
            segmentRanges.append(LineSegmentRange(segmentStart, end));
            break;
        }
        if (resolver.position() == end) {
            end = segmentStart;
            segmentRanges.append(LineSegmentRange(segmentStart, segmentStart));
            resolver.setPositionIgnoringNestedIsolates(segmentStart);
        } else {
            LineSegmentRange range(resolver.position(), end);
            segmentRanges.append(range);
            resolver.setPosition(end, numberOfIsolateAncestors(end));

            if (lineInfo.previousLineBrokeCleanly()) {
                break;
            }
        }
    }
    resolver.setPositionIgnoringNestedIsolates(oldEnd);
    return end;
}
