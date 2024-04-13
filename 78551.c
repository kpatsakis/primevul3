void RenderBlock::layoutRunsAndFloatsInRange(LineLayoutState& layoutState, InlineBidiResolver& resolver, const InlineIterator& cleanLineStart, const BidiStatus& cleanLineBidiStatus, unsigned consecutiveHyphenatedLines)
{
    RenderStyle* styleToUse = style();
    bool paginated = view()->layoutState() && view()->layoutState()->isPaginated();
    LineMidpointState& lineMidpointState = resolver.midpointState();
    InlineIterator end = resolver.position();
    bool checkForEndLineMatch = layoutState.endLine();
    RenderTextInfo renderTextInfo;
    VerticalPositionCache verticalPositionCache;

    LineBreaker lineBreaker(this);

    LayoutUnit absoluteLogicalTop;
    ShapeInsideInfo* shapeInsideInfo = layoutShapeInsideInfo();
    if (shapeInsideInfo) {
        ASSERT(shapeInsideInfo->owner() == this || allowsShapeInsideInfoSharing());
        if (shapeInsideInfo != this->shapeInsideInfo()) {
            absoluteLogicalTop = logicalTop();
        }
        if (logicalHeight() + absoluteLogicalTop < shapeInsideInfo->shapeLogicalTop()) {
            LayoutUnit logicalHeight = shapeInsideInfo->shapeLogicalTop() - absoluteLogicalTop;
            if (layoutState.flowThread())
                logicalHeight -= shapeInsideInfo->owner()->borderAndPaddingBefore();
            setLogicalHeight(logicalHeight);
        }
    }

    while (!end.atEnd()) {
        if (checkForEndLineMatch) {
            layoutState.setEndLineMatched(matchedEndLine(layoutState, resolver, cleanLineStart, cleanLineBidiStatus));
            if (layoutState.endLineMatched()) {
                resolver.setPosition(InlineIterator(resolver.position().root(), 0, 0), 0);
                break;
            }
        }

        lineMidpointState.reset();

        layoutState.lineInfo().setEmpty(true);
        layoutState.lineInfo().resetRunsFromLeadingWhitespace();

        const InlineIterator oldEnd = end;
        bool isNewUBAParagraph = layoutState.lineInfo().previousLineBrokeCleanly();
        FloatingObject* lastFloatFromPreviousLine = (containsFloats()) ? m_floatingObjects->set().last() : 0;

        updateShapeAndSegmentsForCurrentLine(shapeInsideInfo, absoluteLogicalTop, layoutState);

        WordMeasurements wordMeasurements;
        end = lineBreaker.nextLineBreak(resolver, layoutState.lineInfo(), renderTextInfo, lastFloatFromPreviousLine, consecutiveHyphenatedLines, wordMeasurements);
        renderTextInfo.m_lineBreakIterator.resetPriorContext();
        if (resolver.position().atEnd()) {
            resolver.runs().deleteRuns();
            resolver.markCurrentRunEmpty(); // FIXME: This can probably be replaced by an ASSERT (or just removed).
            layoutState.setCheckForFloatsFromLastLine(true);
            resolver.setPosition(InlineIterator(resolver.position().root(), 0, 0), 0);
            break;
        }

        if (adjustLogicalLineTopAndLogicalHeightIfNeeded(shapeInsideInfo, absoluteLogicalTop, layoutState, resolver, lastFloatFromPreviousLine, end, wordMeasurements))
            continue;

        ASSERT(end != resolver.position());

        if (layoutState.lineInfo().isEmpty()) {
            if (lastRootBox())
                lastRootBox()->setLineBreakInfo(end.m_obj, end.m_pos, resolver.status());
        } else {
            VisualDirectionOverride override = (styleToUse->rtlOrdering() == VisualOrder ? (styleToUse->direction() == LTR ? VisualLeftToRightOverride : VisualRightToLeftOverride) : NoVisualOverride);

            if (isNewUBAParagraph && styleToUse->unicodeBidi() == Plaintext && !resolver.context()->parent()) {
                TextDirection direction = determinePlaintextDirectionality(resolver.position().root(), resolver.position().object(), resolver.position().offset());
                resolver.setStatus(BidiStatus(direction, isOverride(styleToUse->unicodeBidi())));
            }
            BidiRunList<BidiRun>& bidiRuns = resolver.runs();
            constructBidiRunsForLine(this, resolver, bidiRuns, end, override, layoutState.lineInfo().previousLineBrokeCleanly());
            ASSERT(resolver.position() == end);

            BidiRun* trailingSpaceRun = !layoutState.lineInfo().previousLineBrokeCleanly() ? handleTrailingSpaces(bidiRuns, resolver.context()) : 0;

            if (bidiRuns.runCount() && lineBreaker.lineWasHyphenated()) {
                bidiRuns.logicallyLastRun()->m_hasHyphen = true;
                consecutiveHyphenatedLines++;
            } else
                consecutiveHyphenatedLines = 0;


            LayoutUnit oldLogicalHeight = logicalHeight();
            RootInlineBox* lineBox = createLineBoxesFromBidiRuns(resolver.status().context->level(), bidiRuns, end, layoutState.lineInfo(), verticalPositionCache, trailingSpaceRun, wordMeasurements);

            bidiRuns.deleteRuns();
            resolver.markCurrentRunEmpty(); // FIXME: This can probably be replaced by an ASSERT (or just removed).

            if (lineBox) {
                lineBox->setLineBreakInfo(end.m_obj, end.m_pos, resolver.status());
                if (layoutState.usesRepaintBounds())
                    layoutState.updateRepaintRangeFromBox(lineBox);

                if (paginated) {
                    LayoutUnit adjustment = 0;
                    adjustLinePositionForPagination(lineBox, adjustment, layoutState.flowThread());
                    if (adjustment) {
                        LayoutUnit oldLineWidth = availableLogicalWidthForLine(oldLogicalHeight, layoutState.lineInfo().isFirstLine());
                        lineBox->adjustBlockDirectionPosition(adjustment);
                        if (layoutState.usesRepaintBounds())
                            layoutState.updateRepaintRangeFromBox(lineBox);

                        if (availableLogicalWidthForLine(oldLogicalHeight + adjustment, layoutState.lineInfo().isFirstLine()) != oldLineWidth) {
                            lineBox->deleteLine();
                            end = restartLayoutRunsAndFloatsInRange(oldLogicalHeight, oldLogicalHeight + adjustment, lastFloatFromPreviousLine, resolver, oldEnd);
                            continue;
                        }

                        setLogicalHeight(lineBox->lineBottomWithLeading());
                    }

                    if (layoutState.flowThread())
                        lineBox->setContainingRegion(regionAtBlockOffset(lineBox->lineTopWithLeading()));
                }
            }
        }

        for (size_t i = 0; i < lineBreaker.positionedObjects().size(); ++i)
            setStaticPositions(this, lineBreaker.positionedObjects()[i]);

        if (!layoutState.lineInfo().isEmpty()) {
            layoutState.lineInfo().setFirstLine(false);
            newLine(lineBreaker.clear());
        }

        if (m_floatingObjects && lastRootBox()) {
            const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
            FloatingObjectSetIterator it = floatingObjectSet.begin();
            FloatingObjectSetIterator end = floatingObjectSet.end();
            if (layoutState.lastFloat()) {
                FloatingObjectSetIterator lastFloatIterator = floatingObjectSet.find(layoutState.lastFloat());
                ASSERT(lastFloatIterator != end);
                ++lastFloatIterator;
                it = lastFloatIterator;
            }
            for (; it != end; ++it) {
                FloatingObject* f = *it;
                appendFloatingObjectToLastLine(f);
                ASSERT(f->renderer() == layoutState.floats()[layoutState.floatIndex()].object);
                if (layoutState.floats()[layoutState.floatIndex()].rect != f->frameRect())
                    checkForEndLineMatch = false;
                layoutState.setFloatIndex(layoutState.floatIndex() + 1);
            }
            layoutState.setLastFloat(!floatingObjectSet.isEmpty() ? floatingObjectSet.last() : 0);
        }

        lineMidpointState.reset();
        resolver.setPosition(end, numberOfIsolateAncestors(end));
    }

    if (paginated && !style()->hasAutoWidows()) {

        RootInlineBox* lineBox = lastRootBox();

        RootInlineBox* firstLineInBlock = firstRootBox();
        int numLinesHanging = 1;
        while (lineBox && lineBox != firstLineInBlock && !lineBox->isFirstAfterPageBreak()) {
            ++numLinesHanging;
            lineBox = lineBox->prevRootBox();
        }

        if (!lineBox || !lineBox->isFirstAfterPageBreak() || lineBox == firstLineInBlock)
            return;

        if (numLinesHanging < style()->widows()) {
            int numLinesNeeded = style()->widows() - numLinesHanging;
            RootInlineBox* currentFirstLineOfNewPage = lineBox;

            lineBox = lineBox->prevRootBox();
            int numLinesInPreviousPage = 1;
            while (lineBox && lineBox != firstLineInBlock && !lineBox->isFirstAfterPageBreak()) {
                ++numLinesInPreviousPage;
                lineBox = lineBox->prevRootBox();
            }

            int orphans = style()->hasAutoOrphans() ? style()->initialOrphans() : style()->orphans();
            int numLinesAvailable = numLinesInPreviousPage - orphans;
            if (numLinesAvailable <= 0)
                return;

            int numLinesToTake = min(numLinesAvailable, numLinesNeeded);
            lineBox = currentFirstLineOfNewPage;
            for (int i = 0; i < numLinesToTake; ++i)
                lineBox = lineBox->prevRootBox();

            setBreakAtLineToAvoidWidow(lineBox);
            markLinesDirtyInBlockRange(lastRootBox()->lineBottomWithLeading(), lineBox->lineBottomWithLeading(), lineBox);
        }
    }
}
