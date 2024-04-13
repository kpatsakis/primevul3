void RenderBlock::LineBreaker::skipTrailingWhitespace(InlineIterator& iterator, const LineInfo& lineInfo)
{
    while (!iterator.atEnd() && !requiresLineBox(iterator, lineInfo, TrailingWhitespace)) {
        RenderObject* object = iterator.m_obj;
        if (object->isOutOfFlowPositioned())
            setStaticPositions(m_block, toRenderBox(object));
        else if (object->isFloating())
            m_block->insertFloatingObject(toRenderBox(object));
        iterator.increment();
    }
}
