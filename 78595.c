void RenderBlock::LineBreaker::skipLeadingWhitespace(InlineBidiResolver& resolver, LineInfo& lineInfo,
                                                     FloatingObject* lastFloatFromPreviousLine, LineWidth& width)
{
    while (!resolver.position().atEnd() && !requiresLineBox(resolver.position(), lineInfo, LeadingWhitespace)) {
        RenderObject* object = resolver.position().m_obj;
        if (object->isOutOfFlowPositioned()) {
            setStaticPositions(m_block, toRenderBox(object));
            if (object->style()->isOriginalDisplayInlineType()) {
                resolver.runs().addRun(createRun(0, 1, object, resolver));
                lineInfo.incrementRunsFromLeadingWhitespace();
            }
        } else if (object->isFloating()) {
            LayoutUnit marginOffset = (!object->previousSibling() && m_block->isSelfCollapsingBlock() && m_block->style()->clear() && m_block->getClearDelta(m_block, LayoutUnit())) ? m_block->collapsedMarginBeforeForChild(m_block) : LayoutUnit();
            LayoutUnit oldLogicalHeight = m_block->logicalHeight();
            m_block->setLogicalHeight(oldLogicalHeight + marginOffset);
            m_block->positionNewFloatOnLine(m_block->insertFloatingObject(toRenderBox(object)), lastFloatFromPreviousLine, lineInfo, width);
            m_block->setLogicalHeight(oldLogicalHeight);
        } else if (object->isText() && object->style()->hasTextCombine() && object->isCombineText() && !toRenderCombineText(object)->isCombined()) {
            toRenderCombineText(object)->combineText();
            if (toRenderCombineText(object)->isCombined())
                continue;
        }
        resolver.increment();
    }
    resolver.commitExplicitEmbedding();
}
