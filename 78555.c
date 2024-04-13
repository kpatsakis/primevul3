void RenderBlock::linkToEndLineIfNeeded(LineLayoutState& layoutState)
{
    if (layoutState.endLine()) {
        if (layoutState.endLineMatched()) {
            bool paginated = view()->layoutState() && view()->layoutState()->isPaginated();
            LayoutUnit delta = logicalHeight() - layoutState.endLineLogicalTop();
            for (RootInlineBox* line = layoutState.endLine(); line; line = line->nextRootBox()) {
                line->attachLine();
                if (paginated) {
                    delta -= line->paginationStrut();
                    adjustLinePositionForPagination(line, delta, layoutState.flowThread());
                }
                if (delta) {
                    layoutState.updateRepaintRangeFromBox(line, delta);
                    line->adjustBlockDirectionPosition(delta);
                }
                if (layoutState.flowThread())
                    line->setContainingRegion(regionAtBlockOffset(line->lineTopWithLeading()));
                if (Vector<RenderBox*>* cleanLineFloats = line->floatsPtr()) {
                    Vector<RenderBox*>::iterator end = cleanLineFloats->end();
                    for (Vector<RenderBox*>::iterator f = cleanLineFloats->begin(); f != end; ++f) {
                        FloatingObject* floatingObject = insertFloatingObject(*f);
                        ASSERT(!floatingObject->originatingLine());
                        floatingObject->setOriginatingLine(line);
                        setLogicalHeight(logicalTopForChild(*f) - marginBeforeForChild(*f) + delta);
                        positionNewFloats();
                    }
                }
            }
            setLogicalHeight(lastRootBox()->lineBottomWithLeading());
        } else {
            deleteLineRange(layoutState, layoutState.endLine());
        }
    }

    if (m_floatingObjects && (layoutState.checkForFloatsFromLastLine() || positionNewFloats()) && lastRootBox()) {
        if (layoutState.checkForFloatsFromLastLine()) {
            LayoutUnit bottomVisualOverflow = lastRootBox()->logicalBottomVisualOverflow();
            LayoutUnit bottomLayoutOverflow = lastRootBox()->logicalBottomLayoutOverflow();
            TrailingFloatsRootInlineBox* trailingFloatsLineBox = new TrailingFloatsRootInlineBox(this);
            m_lineBoxes.appendLineBox(trailingFloatsLineBox);
            trailingFloatsLineBox->setConstructed();
            GlyphOverflowAndFallbackFontsMap textBoxDataMap;
            VerticalPositionCache verticalPositionCache;
            LayoutUnit blockLogicalHeight = logicalHeight();
            trailingFloatsLineBox->alignBoxesInBlockDirection(blockLogicalHeight, textBoxDataMap, verticalPositionCache);
            trailingFloatsLineBox->setLineTopBottomPositions(blockLogicalHeight, blockLogicalHeight, blockLogicalHeight, blockLogicalHeight);
            trailingFloatsLineBox->setPaginatedLineWidth(availableLogicalWidthForContent(blockLogicalHeight));
            LayoutRect logicalLayoutOverflow(0, blockLogicalHeight, 1, bottomLayoutOverflow - blockLogicalHeight);
            LayoutRect logicalVisualOverflow(0, blockLogicalHeight, 1, bottomVisualOverflow - blockLogicalHeight);
            trailingFloatsLineBox->setOverflowFromLogicalRects(logicalLayoutOverflow, logicalVisualOverflow, trailingFloatsLineBox->lineTop(), trailingFloatsLineBox->lineBottom());
            if (layoutState.flowThread())
                trailingFloatsLineBox->setContainingRegion(regionAtBlockOffset(trailingFloatsLineBox->lineTopWithLeading()));
        }

        const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
        FloatingObjectSetIterator it = floatingObjectSet.begin();
        FloatingObjectSetIterator end = floatingObjectSet.end();
        if (layoutState.lastFloat()) {
            FloatingObjectSetIterator lastFloatIterator = floatingObjectSet.find(layoutState.lastFloat());
            ASSERT(lastFloatIterator != end);
            ++lastFloatIterator;
            it = lastFloatIterator;
        }
        for (; it != end; ++it)
            appendFloatingObjectToLastLine(*it);
        layoutState.setLastFloat(!floatingObjectSet.isEmpty() ? floatingObjectSet.last() : 0);
    }
}
