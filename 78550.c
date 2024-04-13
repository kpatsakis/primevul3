void RenderBlock::layoutRunsAndFloats(LineLayoutState& layoutState, bool hasInlineChild)
{
    InlineBidiResolver resolver;
    RootInlineBox* startLine = determineStartPosition(layoutState, resolver);

    unsigned consecutiveHyphenatedLines = 0;
    if (startLine) {
        for (RootInlineBox* line = startLine->prevRootBox(); line && line->isHyphenated(); line = line->prevRootBox())
            consecutiveHyphenatedLines++;
    }

    if (layoutState.isFullLayout() && hasInlineChild && !selfNeedsLayout()) {
        setNeedsLayout(MarkOnlyThis); // Mark as needing a full layout to force us to repaint.
        RenderView* v = view();
        if (v && !v->doingFullRepaint() && hasLayer()) {
            repaintUsingContainer(containerForRepaint(), pixelSnappedIntRect(layer()->repaintRect()));
        }
    }

    if (containsFloats())
        layoutState.setLastFloat(m_floatingObjects->set().last());

    InlineIterator cleanLineStart;
    BidiStatus cleanLineBidiStatus;
    if (!layoutState.isFullLayout() && startLine)
        determineEndPosition(layoutState, startLine, cleanLineStart, cleanLineBidiStatus);

    if (startLine) {
        if (!layoutState.usesRepaintBounds())
            layoutState.setRepaintRange(logicalHeight());
        deleteLineRange(layoutState, startLine);
    }

    if (!layoutState.isFullLayout() && lastRootBox() && lastRootBox()->endsWithBreak()) {
        if (InlineBox* lastLeafChild = lastRootBox()->lastLeafChild()) {
            RenderObject* lastObject = lastLeafChild->renderer();
            if (!lastObject->isBR())
                lastObject = lastRootBox()->firstLeafChild()->renderer();
            if (lastObject->isBR()) {
                EClear clear = lastObject->style()->clear();
                if (clear != CNONE)
                    newLine(clear);
            }
        }
    }

    layoutRunsAndFloatsInRange(layoutState, resolver, cleanLineStart, cleanLineBidiStatus, consecutiveHyphenatedLines);
    linkToEndLineIfNeeded(layoutState);
    repaintDirtyFloats(layoutState.floats());
}
