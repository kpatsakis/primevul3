void LineWidth::fitBelowFloats()
{
    ASSERT(!m_committedWidth);
    ASSERT(!fitsOnLine());

    LayoutUnit floatLogicalBottom;
    LayoutUnit lastFloatLogicalBottom = m_block->logicalHeight();
    float newLineWidth = m_availableWidth;
    float newLineLeft = m_left;
    float newLineRight = m_right;
    while (true) {
        floatLogicalBottom = m_block->nextFloatLogicalBottomBelow(lastFloatLogicalBottom);
        if (floatLogicalBottom <= lastFloatLogicalBottom)
            break;

        newLineLeft = m_block->logicalLeftOffsetForLine(floatLogicalBottom, shouldIndentText());
        newLineRight = m_block->logicalRightOffsetForLine(floatLogicalBottom, shouldIndentText());
        newLineWidth = max(0.0f, newLineRight - newLineLeft);
        lastFloatLogicalBottom = floatLogicalBottom;
        if (newLineWidth >= m_uncommittedWidth)
            break;
    }

    if (newLineWidth > m_availableWidth) {
        m_block->setLogicalHeight(lastFloatLogicalBottom);
        m_availableWidth = newLineWidth + m_overhangWidth;
        m_left = newLineLeft;
        m_right = newLineRight;
    }
}
