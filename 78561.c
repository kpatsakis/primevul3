InlineIterator RenderBlock::LineBreaker::nextSegmentBreak(InlineBidiResolver& resolver, LineInfo& lineInfo, RenderTextInfo& renderTextInfo, FloatingObject* lastFloatFromPreviousLine, unsigned consecutiveHyphenatedLines, WordMeasurements& wordMeasurements)
{
    reset();

    ASSERT(resolver.position().root() == m_block);

    bool appliedStartWidth = resolver.position().m_pos > 0;
    bool includeEndWidth = true;
    LineMidpointState& lineMidpointState = resolver.midpointState();

    LineWidth width(m_block, lineInfo.isFirstLine(), requiresIndent(lineInfo.isFirstLine(), lineInfo.previousLineBrokeCleanly(), m_block->style()));

    skipLeadingWhitespace(resolver, lineInfo, lastFloatFromPreviousLine, width);

    if (resolver.position().atEnd())
        return resolver.position();

    bool ignoringSpaces = false;
    InlineIterator ignoreStart;

    bool currentCharacterIsSpace = false;
    bool currentCharacterShouldCollapseIfPreWap = false;
    TrailingObjects trailingObjects;

    InlineIterator lBreak = resolver.position();

    InlineIterator current = resolver.position();
    RenderObject* last = current.m_obj;
    bool atStart = true;

    bool startingNewParagraph = lineInfo.previousLineBrokeCleanly();
    lineInfo.setPreviousLineBrokeCleanly(false);

    bool autoWrapWasEverTrueOnLine = false;
    bool floatsFitOnLine = true;

    RenderStyle* blockStyle = m_block->style();
    bool allowImagesToBreak = !m_block->document().inQuirksMode() || !m_block->isTableCell() || !blockStyle->logicalWidth().isIntrinsicOrAuto();

    EWhiteSpace currWS = blockStyle->whiteSpace();
    EWhiteSpace lastWS = currWS;
    while (current.m_obj) {
        RenderStyle* currentStyle = current.m_obj->style();
        RenderObject* next = bidiNextSkippingEmptyInlines(m_block, current.m_obj);
        if (next && next->parent() && !next->parent()->isDescendantOf(current.m_obj->parent()))
            includeEndWidth = true;

        currWS = current.m_obj->isReplaced() ? current.m_obj->parent()->style()->whiteSpace() : currentStyle->whiteSpace();
        lastWS = last->isReplaced() ? last->parent()->style()->whiteSpace() : last->style()->whiteSpace();

        bool autoWrap = RenderStyle::autoWrap(currWS);
        autoWrapWasEverTrueOnLine = autoWrapWasEverTrueOnLine || autoWrap;

        bool preserveNewline = current.m_obj->isSVGInlineText() ? false : RenderStyle::preserveNewline(currWS);

        bool collapseWhiteSpace = RenderStyle::collapseWhiteSpace(currWS);

        if (current.m_obj->isBR()) {
            if (width.fitsOnLine()) {
                lBreak.moveToStartOf(current.m_obj);
                lBreak.increment();

                if (startingNewParagraph)
                    lineInfo.setEmpty(false, m_block, &width);
                trailingObjects.clear();
                lineInfo.setPreviousLineBrokeCleanly(true);

                if (ignoringSpaces && currentStyle->clear() != CNONE)
                    ensureLineBoxInsideIgnoredSpaces(lineMidpointState, current.m_obj);

                if (!lineInfo.isEmpty())
                    m_clear = currentStyle->clear();
            }
            goto end;
        }

        if (current.m_obj->isOutOfFlowPositioned()) {
            RenderBox* box = toRenderBox(current.m_obj);
            bool isInlineType = box->style()->isOriginalDisplayInlineType();
            if (!isInlineType)
                m_block->setStaticInlinePositionForChild(box, m_block->logicalHeight(), m_block->startOffsetForContent(m_block->logicalHeight()));
            else  {
                box->layer()->setStaticBlockPosition(m_block->logicalHeight());
            }

            if (isInlineType || current.m_obj->container()->isRenderInline()) {
                if (ignoringSpaces)
                    ensureLineBoxInsideIgnoredSpaces(lineMidpointState, current.m_obj);
                trailingObjects.appendBoxIfNeeded(box);
            } else
                m_positionedObjects.append(box);
            width.addUncommittedWidth(inlineLogicalWidth(current.m_obj));
            renderTextInfo.m_lineBreakIterator.resetPriorContext();
        } else if (current.m_obj->isFloating()) {
            RenderBox* floatBox = toRenderBox(current.m_obj);
            FloatingObject* f = m_block->insertFloatingObject(floatBox);
            if (floatsFitOnLine && width.fitsOnLine(f->logicalWidth(m_block->isHorizontalWritingMode()))) {
                m_block->positionNewFloatOnLine(f, lastFloatFromPreviousLine, lineInfo, width);
                if (lBreak.m_obj == current.m_obj) {
                    ASSERT(!lBreak.m_pos);
                    lBreak.increment();
                }
            } else
                floatsFitOnLine = false;
            renderTextInfo.m_lineBreakIterator.updatePriorContext(replacementCharacter);
        } else if (current.m_obj->isRenderInline()) {
            ASSERT(isEmptyInline(current.m_obj));

            RenderInline* flowBox = toRenderInline(current.m_obj);

            bool requiresLineBox = alwaysRequiresLineBox(current.m_obj);
            if (requiresLineBox || requiresLineBoxForContent(flowBox, lineInfo)) {
                if (requiresLineBox)
                    lineInfo.setEmpty(false, m_block, &width);
                if (ignoringSpaces) {
                    trailingObjects.clear();
                    ensureLineBoxInsideIgnoredSpaces(lineMidpointState, current.m_obj);
                } else if (blockStyle->collapseWhiteSpace() && resolver.position().m_obj == current.m_obj
                    && shouldSkipWhitespaceAfterStartObject(m_block, current.m_obj, lineMidpointState)) {
                    currentCharacterShouldCollapseIfPreWap = currentCharacterIsSpace = true;
                    ignoringSpaces = true;
                }
            }

            width.addUncommittedWidth(inlineLogicalWidth(current.m_obj) + borderPaddingMarginStart(flowBox) + borderPaddingMarginEnd(flowBox));
        } else if (current.m_obj->isReplaced()) {
            RenderBox* replacedBox = toRenderBox(current.m_obj);

            if (atStart)
                width.updateAvailableWidth(replacedBox->logicalHeight());

            if ((autoWrap || RenderStyle::autoWrap(lastWS)) && (!current.m_obj->isImage() || allowImagesToBreak)) {
                width.commit();
                lBreak.moveToStartOf(current.m_obj);
            }

            if (ignoringSpaces)
                stopIgnoringSpaces(lineMidpointState, InlineIterator(0, current.m_obj, 0));

            lineInfo.setEmpty(false, m_block, &width);
            ignoringSpaces = false;
            currentCharacterShouldCollapseIfPreWap = currentCharacterIsSpace = false;
            trailingObjects.clear();

            LayoutUnit replacedLogicalWidth = m_block->logicalWidthForChild(replacedBox) + m_block->marginStartForChild(replacedBox) + m_block->marginEndForChild(replacedBox) + inlineLogicalWidth(current.m_obj);
            if (current.m_obj->isListMarker()) {
                if (blockStyle->collapseWhiteSpace() && shouldSkipWhitespaceAfterStartObject(m_block, current.m_obj, lineMidpointState)) {
                    currentCharacterShouldCollapseIfPreWap = currentCharacterIsSpace = true;
                    ignoringSpaces = true;
                }
                if (toRenderListMarker(current.m_obj)->isInside())
                    width.addUncommittedWidth(replacedLogicalWidth);
            } else
                width.addUncommittedWidth(replacedLogicalWidth);
            if (current.m_obj->isRubyRun())
                width.applyOverhang(toRenderRubyRun(current.m_obj), last, next);
            renderTextInfo.m_lineBreakIterator.updatePriorContext(replacementCharacter);
        } else if (current.m_obj->isText()) {
            if (!current.m_pos)
                appliedStartWidth = false;

            RenderText* t = toRenderText(current.m_obj);

            bool isSVGText = t->isSVGInlineText();

            if (t->style()->hasTextCombine() && current.m_obj->isCombineText() && !toRenderCombineText(current.m_obj)->isCombined()) {
                RenderCombineText* combineRenderer = toRenderCombineText(current.m_obj);
                combineRenderer->combineText();
                if (iteratorIsBeyondEndOfRenderCombineText(lBreak, combineRenderer)) {
                    ASSERT(iteratorIsBeyondEndOfRenderCombineText(resolver.position(), combineRenderer));
                    lBreak.increment();
                    resolver.increment();
                }
            }

            RenderStyle* style = t->style(lineInfo.isFirstLine());
            const Font& f = style->font();
            bool isFixedPitch = f.isFixedPitch();

            unsigned lastSpace = current.m_pos;
            float wordSpacing = currentStyle->wordSpacing();
            float lastSpaceWordSpacing = 0;
            float wordSpacingForWordMeasurement = 0;

            float wrapW = width.uncommittedWidth() + inlineLogicalWidth(current.m_obj, !appliedStartWidth, true);
            float charWidth = 0;
            bool breakWords = currentStyle->breakWords() && ((autoWrap && !width.committedWidth()) || currWS == PRE);
            bool midWordBreak = false;
            bool breakAll = currentStyle->wordBreak() == BreakAllWordBreak && autoWrap;
            float hyphenWidth = 0;

            if (isSVGText) {
                breakWords = false;
                breakAll = false;
            }

            if (t->isWordBreak()) {
                width.commit();
                lBreak.moveToStartOf(current.m_obj);
                ASSERT(current.m_pos == t->textLength());
            }

            if (renderTextInfo.m_text != t) {
                renderTextInfo.m_text = t;
                renderTextInfo.m_font = &f;
                renderTextInfo.m_layout = f.createLayout(t, width.currentWidth(), collapseWhiteSpace);
                renderTextInfo.m_lineBreakIterator.resetStringAndReleaseIterator(t->text(), style->locale());
            } else if (renderTextInfo.m_layout && renderTextInfo.m_font != &f) {
                renderTextInfo.m_font = &f;
                renderTextInfo.m_layout = f.createLayout(t, width.currentWidth(), collapseWhiteSpace);
            }

            TextLayout* textLayout = renderTextInfo.m_layout.get();

            float wordTrailingSpaceWidth = (f.typesettingFeatures() & Kerning) && !textLayout ? f.width(constructTextRun(t, f, &space, 1, style)) + wordSpacing : 0;

            UChar lastCharacter = renderTextInfo.m_lineBreakIterator.lastCharacter();
            UChar secondToLastCharacter = renderTextInfo.m_lineBreakIterator.secondToLastCharacter();
            for (; current.m_pos < t->textLength(); current.fastIncrementInTextNode()) {
                bool previousCharacterIsSpace = currentCharacterIsSpace;
                bool previousCharacterShouldCollapseIfPreWap = currentCharacterShouldCollapseIfPreWap;
                UChar c = current.current();
                currentCharacterShouldCollapseIfPreWap = currentCharacterIsSpace = c == ' ' || c == '\t' || (!preserveNewline && (c == '\n'));

                if (!collapseWhiteSpace || !currentCharacterIsSpace)
                    lineInfo.setEmpty(false, m_block, &width);

                if (c == softHyphen && autoWrap && !hyphenWidth) {
                    hyphenWidth = measureHyphenWidth(t, f);
                    width.addUncommittedWidth(hyphenWidth);
                }

                bool applyWordSpacing = false;

                if ((breakAll || breakWords) && !midWordBreak) {
                    wrapW += charWidth;
                    bool midWordBreakIsBeforeSurrogatePair = U16_IS_LEAD(c) && current.m_pos + 1 < t->textLength() && U16_IS_TRAIL((*t)[current.m_pos + 1]);
                    charWidth = textWidth(t, current.m_pos, midWordBreakIsBeforeSurrogatePair ? 2 : 1, f, width.committedWidth() + wrapW, isFixedPitch, collapseWhiteSpace, 0, textLayout);
                    midWordBreak = width.committedWidth() + wrapW + charWidth > width.availableWidth();
                }

                bool betweenWords = c == '\n' || (currWS != PRE && !atStart && isBreakable(renderTextInfo.m_lineBreakIterator, current.m_pos, current.m_nextBreakablePosition));

                if (betweenWords || midWordBreak) {
                    bool stoppedIgnoringSpaces = false;
                    if (ignoringSpaces) {
                        lastSpaceWordSpacing = 0;
                        if (!currentCharacterIsSpace) {
                            ignoringSpaces = false;
                            wordSpacingForWordMeasurement = 0;
                            lastSpace = current.m_pos; // e.g., "Foo    goo", don't add in any of the ignored spaces.
                            stopIgnoringSpaces(lineMidpointState, InlineIterator(0, current.m_obj, current.m_pos));
                            stoppedIgnoringSpaces = true;
                        } else {
                            goto nextCharacter;
                        }
                    }

                    wordMeasurements.grow(wordMeasurements.size() + 1);
                    WordMeasurement& wordMeasurement = wordMeasurements.last();

                    wordMeasurement.renderer = t;
                    wordMeasurement.endOffset = current.m_pos;
                    wordMeasurement.startOffset = lastSpace;

                    float additionalTmpW;
                    if (wordTrailingSpaceWidth && c == ' ')
                        additionalTmpW = textWidth(t, lastSpace, current.m_pos + 1 - lastSpace, f, width.currentWidth(), isFixedPitch, collapseWhiteSpace, &wordMeasurement.fallbackFonts, textLayout) - wordTrailingSpaceWidth;
                    else
                        additionalTmpW = textWidth(t, lastSpace, current.m_pos - lastSpace, f, width.currentWidth(), isFixedPitch, collapseWhiteSpace, &wordMeasurement.fallbackFonts, textLayout);

                    wordMeasurement.width = additionalTmpW + wordSpacingForWordMeasurement;
                    additionalTmpW += lastSpaceWordSpacing;
                    width.addUncommittedWidth(additionalTmpW);
                    if (!appliedStartWidth) {
                        width.addUncommittedWidth(inlineLogicalWidth(current.m_obj, true, false));
                        appliedStartWidth = true;
                    }

                    applyWordSpacing = wordSpacing && currentCharacterIsSpace;

                    if (!width.committedWidth() && autoWrap && !width.fitsOnLine())
                        width.fitBelowFloats();

                    if (autoWrap || breakWords) {
                        bool lineWasTooWide = false;
                        if (width.fitsOnLine() && currentCharacterIsSpace && currentStyle->breakOnlyAfterWhiteSpace() && !midWordBreak) {
                            float charWidth = textWidth(t, current.m_pos, 1, f, width.currentWidth(), isFixedPitch, collapseWhiteSpace, &wordMeasurement.fallbackFonts, textLayout) + (applyWordSpacing ? wordSpacing : 0);
                            if (!width.fitsOnLine(charWidth)) {
                                lineWasTooWide = true;
                                lBreak.moveTo(current.m_obj, current.m_pos, current.m_nextBreakablePosition);
                                skipTrailingWhitespace(lBreak, lineInfo);
                            }
                        }
                        if (lineWasTooWide || !width.fitsOnLine()) {
                            if (lBreak.atTextParagraphSeparator()) {
                                if (!stoppedIgnoringSpaces && current.m_pos > 0)
                                    ensureCharacterGetsLineBox(lineMidpointState, current);
                                lBreak.increment();
                                lineInfo.setPreviousLineBrokeCleanly(true);
                                wordMeasurement.endOffset = lBreak.m_pos;
                            }
                            if (lBreak.m_obj && lBreak.m_pos && lBreak.m_obj->isText() && toRenderText(lBreak.m_obj)->textLength() && toRenderText(lBreak.m_obj)->characterAt(lBreak.m_pos - 1) == softHyphen)
                                m_hyphenated = true;
                            if (lBreak.m_pos && lBreak.m_pos != (unsigned)wordMeasurement.endOffset && !wordMeasurement.width) {
                                if (charWidth) {
                                    wordMeasurement.endOffset = lBreak.m_pos;
                                    wordMeasurement.width = charWidth;
                                }
                            }
                            if (ignoringSpaces || !collapseWhiteSpace || !currentCharacterIsSpace || !previousCharacterIsSpace)
                                goto end;
                        } else {
                            if (!betweenWords || (midWordBreak && !autoWrap))
                                width.addUncommittedWidth(-additionalTmpW);
                            if (hyphenWidth) {
                                width.addUncommittedWidth(-hyphenWidth);
                                hyphenWidth = 0;
                            }
                        }
                    }

                    if (c == '\n' && preserveNewline) {
                        if (!stoppedIgnoringSpaces && current.m_pos > 0)
                            ensureCharacterGetsLineBox(lineMidpointState, current);
                        lBreak.moveTo(current.m_obj, current.m_pos, current.m_nextBreakablePosition);
                        lBreak.increment();
                        lineInfo.setPreviousLineBrokeCleanly(true);
                        return lBreak;
                    }

                    if (autoWrap && betweenWords) {
                        width.commit();
                        wrapW = 0;
                        lBreak.moveTo(current.m_obj, current.m_pos, current.m_nextBreakablePosition);
                        breakWords = false;
                    }

                    if (midWordBreak && !U16_IS_TRAIL(c) && !(category(c) & (Mark_NonSpacing | Mark_Enclosing | Mark_SpacingCombining))) {
                        lBreak.moveTo(current.m_obj, current.m_pos, current.m_nextBreakablePosition);
                        midWordBreak &= (breakWords || breakAll);
                    }

                    if (betweenWords) {
                        lastSpaceWordSpacing = applyWordSpacing ? wordSpacing : 0;
                        wordSpacingForWordMeasurement = (applyWordSpacing && wordMeasurement.width) ? wordSpacing : 0;
                        lastSpace = current.m_pos;
                    }

                    if (!ignoringSpaces && currentStyle->collapseWhiteSpace()) {
                        if (currentCharacterIsSpace && previousCharacterIsSpace) {
                            ignoringSpaces = true;

                            startIgnoringSpaces(lineMidpointState, ignoreStart);
                            trailingObjects.updateMidpointsForTrailingBoxes(lineMidpointState, InlineIterator(), TrailingObjects::DoNotCollapseFirstSpace);
                        }
                    }
                } else if (ignoringSpaces) {
                    ignoringSpaces = false;
                    lastSpaceWordSpacing = applyWordSpacing ? wordSpacing : 0;
                    wordSpacingForWordMeasurement = (applyWordSpacing && wordMeasurements.last().width) ? wordSpacing : 0;
                    lastSpace = current.m_pos; // e.g., "Foo    goo", don't add in any of the ignored spaces.
                    stopIgnoringSpaces(lineMidpointState, InlineIterator(0, current.m_obj, current.m_pos));
                }

                if (isSVGText && current.m_pos > 0) {
                    if (toRenderSVGInlineText(t)->characterStartsNewTextChunk(current.m_pos))
                        ensureCharacterGetsLineBox(lineMidpointState, current);
                }

                if (currentCharacterIsSpace && !previousCharacterIsSpace) {
                    ignoreStart.m_obj = current.m_obj;
                    ignoreStart.m_pos = current.m_pos;
                }

                if (!currentCharacterIsSpace && previousCharacterShouldCollapseIfPreWap) {
                    if (autoWrap && currentStyle->breakOnlyAfterWhiteSpace())
                        lBreak.moveTo(current.m_obj, current.m_pos, current.m_nextBreakablePosition);
                }

                if (collapseWhiteSpace && currentCharacterIsSpace && !ignoringSpaces)
                    trailingObjects.setTrailingWhitespace(toRenderText(current.m_obj));
                else if (!currentStyle->collapseWhiteSpace() || !currentCharacterIsSpace)
                    trailingObjects.clear();

                atStart = false;
            nextCharacter:
                secondToLastCharacter = lastCharacter;
                lastCharacter = c;
            }

            renderTextInfo.m_lineBreakIterator.setPriorContext(lastCharacter, secondToLastCharacter);

            wordMeasurements.grow(wordMeasurements.size() + 1);
            WordMeasurement& wordMeasurement = wordMeasurements.last();
            wordMeasurement.renderer = t;

            float additionalTmpW = ignoringSpaces ? 0 : textWidth(t, lastSpace, current.m_pos - lastSpace, f, width.currentWidth(), isFixedPitch, collapseWhiteSpace, &wordMeasurement.fallbackFonts, textLayout);
            wordMeasurement.startOffset = lastSpace;
            wordMeasurement.endOffset = current.m_pos;
            wordMeasurement.width = ignoringSpaces ? 0 : additionalTmpW + wordSpacingForWordMeasurement;
            additionalTmpW += lastSpaceWordSpacing;
            width.addUncommittedWidth(additionalTmpW + inlineLogicalWidth(current.m_obj, !appliedStartWidth, includeEndWidth));
            includeEndWidth = false;

            if (!width.fitsOnLine()) {
                if (!m_hyphenated && lBreak.previousInSameNode() == softHyphen)
                    m_hyphenated = true;

                if (m_hyphenated)
                    goto end;
            }
        } else
            ASSERT_NOT_REACHED();

        bool checkForBreak = autoWrap;
        if (width.committedWidth() && !width.fitsOnLine() && lBreak.m_obj && currWS == NOWRAP)
            checkForBreak = true;
        else if (next && current.m_obj->isText() && next->isText() && !next->isBR() && (autoWrap || next->style()->autoWrap())) {
            if (autoWrap && currentCharacterIsSpace)
                checkForBreak = true;
            else {
                RenderText* nextText = toRenderText(next);
                if (nextText->textLength()) {
                    UChar c = nextText->characterAt(0);
                    checkForBreak = !currentCharacterIsSpace && (c == ' ' || c == '\t' || (c == '\n' && !next->preservesNewline()));
                } else if (nextText->isWordBreak())
                    checkForBreak = true;

                if (!width.fitsOnLine() && !width.committedWidth())
                    width.fitBelowFloats();

                bool canPlaceOnLine = width.fitsOnLine() || !autoWrapWasEverTrueOnLine;
                if (canPlaceOnLine && checkForBreak) {
                    width.commit();
                    lBreak.moveToStartOf(next);
                }
            }
        }

        if (checkForBreak && !width.fitsOnLine()) {
            if (currentCharacterIsSpace && !ignoringSpaces && currentStyle->collapseWhiteSpace())
                trailingObjects.clear();

            if (width.committedWidth())
                goto end;

            width.fitBelowFloats();

            if (!width.fitsOnLine())
                goto end;
        } else if (blockStyle->autoWrap() && !width.fitsOnLine() && !width.committedWidth()) {
            width.fitBelowFloats();
        }

        if (!current.m_obj->isFloatingOrOutOfFlowPositioned()) {
            last = current.m_obj;
            if (last->isReplaced() && autoWrap && (!last->isImage() || allowImagesToBreak) && (!last->isListMarker() || toRenderListMarker(last)->isInside())) {
                width.commit();
                lBreak.moveToStartOf(next);
            }
        }

        if (!collapseWhiteSpace)
            currentCharacterIsSpace = false;

        current.moveToStartOf(next);
        atStart = false;
    }

    if (width.fitsOnLine() || lastWS == NOWRAP)
        lBreak.clear();

 end:
    ShapeInsideInfo* shapeInfo = m_block->layoutShapeInsideInfo();
    bool segmentAllowsOverflow = !shapeInfo || !shapeInfo->hasSegments();

    if (lBreak == resolver.position() && (!lBreak.m_obj || !lBreak.m_obj->isBR()) && segmentAllowsOverflow) {
        if (blockStyle->whiteSpace() == PRE && !current.m_pos) {
            lBreak.moveTo(last, last->isText() ? last->length() : 0);
        } else if (lBreak.m_obj) {
            lBreak.moveTo(current.m_obj, current.m_pos);
        }
    }

    if (lBreak == resolver.position() && segmentAllowsOverflow)
        lBreak.increment();

    checkMidpoints(lineMidpointState, lBreak);

    trailingObjects.updateMidpointsForTrailingBoxes(lineMidpointState, lBreak, TrailingObjects::CollapseFirstSpace);

    if (lBreak.m_pos > 0) {
        lBreak.m_pos--;
        lBreak.increment();
    }

    return lBreak;
}
