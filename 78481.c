    LineWidth(RenderBlock* block, bool isFirstLine, IndentTextOrNot shouldIndentText)
        : m_block(block)
        , m_uncommittedWidth(0)
        , m_committedWidth(0)
        , m_overhangWidth(0)
        , m_left(0)
        , m_right(0)
        , m_availableWidth(0)
        , m_segment(0)
        , m_isFirstLine(isFirstLine)
        , m_shouldIndentText(shouldIndentText)
    {
        ASSERT(block);
        ShapeInsideInfo* shapeInsideInfo = m_block->layoutShapeInsideInfo();
        if (shapeInsideInfo)
            m_segment = shapeInsideInfo->currentSegment();
        updateAvailableWidth();
    }
