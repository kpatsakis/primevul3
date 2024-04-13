inline void LineWidth::shrinkAvailableWidthForNewFloatIfNeeded(FloatingObject* newFloat)
{
    LayoutUnit height = m_block->logicalHeight();
    if (height < newFloat->logicalTop(m_block->isHorizontalWritingMode()) || height >= newFloat->logicalBottom(m_block->isHorizontalWritingMode()))
        return;

    ShapeOutsideInfo* previousShapeOutsideInfo = 0;
    const FloatingObjectSet& floatingObjectSet = m_block->m_floatingObjects->set();
    FloatingObjectSetIterator it = floatingObjectSet.end();
    FloatingObjectSetIterator begin = floatingObjectSet.begin();
    while (it != begin) {
        --it;
        FloatingObject* previousFloat = *it;
        if (previousFloat != newFloat && previousFloat->type() == newFloat->type()) {
            previousShapeOutsideInfo = previousFloat->renderer()->shapeOutsideInfo();
            if (previousShapeOutsideInfo) {
                previousShapeOutsideInfo->computeSegmentsForContainingBlockLine(m_block->logicalHeight(), previousFloat->logicalTop(m_block->isHorizontalWritingMode()), logicalHeightForLine(m_block, m_isFirstLine));
            }
            break;
        }
    }

    ShapeOutsideInfo* shapeOutsideInfo = newFloat->renderer()->shapeOutsideInfo();
    if (shapeOutsideInfo)
        shapeOutsideInfo->computeSegmentsForContainingBlockLine(m_block->logicalHeight(), newFloat->logicalTop(m_block->isHorizontalWritingMode()), logicalHeightForLine(m_block, m_isFirstLine));

    if (newFloat->type() == FloatingObject::FloatLeft) {
        float newLeft = newFloat->logicalRight(m_block->isHorizontalWritingMode());
        if (previousShapeOutsideInfo)
            newLeft -= previousShapeOutsideInfo->rightSegmentMarginBoxDelta();
        if (shapeOutsideInfo)
            newLeft += shapeOutsideInfo->rightSegmentMarginBoxDelta();

        if (shouldIndentText() && m_block->style()->isLeftToRightDirection())
            newLeft += floorToInt(m_block->textIndentOffset());
        m_left = max<float>(m_left, newLeft);
    } else {
        float newRight = newFloat->logicalLeft(m_block->isHorizontalWritingMode());
        if (previousShapeOutsideInfo)
            newRight -= previousShapeOutsideInfo->leftSegmentMarginBoxDelta();
        if (shapeOutsideInfo)
            newRight += shapeOutsideInfo->leftSegmentMarginBoxDelta();

        if (shouldIndentText() && !m_block->style()->isLeftToRightDirection())
            newRight -= floorToInt(m_block->textIndentOffset());
        m_right = min<float>(m_right, newRight);
    }

    computeAvailableWidthFromLeftAndRight();
}
