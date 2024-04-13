static inline LayoutUnit adjustLogicalLineTop(ShapeInsideInfo* shapeInsideInfo, InlineIterator start, InlineIterator end, const WordMeasurements& wordMeasurements)
{
    if (!shapeInsideInfo || end != start)
        return 0;

    float minWidth = firstPositiveWidth(wordMeasurements);
    ASSERT(minWidth || wordMeasurements.isEmpty());
    if (minWidth > 0 && shapeInsideInfo->adjustLogicalLineTop(minWidth))
        return shapeInsideInfo->logicalLineTop();

    return shapeInsideInfo->shapeLogicalBottom();
}
