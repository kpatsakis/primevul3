void InlineFlowBox::placeBoxesInBlockDirection(int top, int maxHeight, int maxAscent, bool strictMode, int& lineTop, int& lineBottom, bool& setLineTop,
                                               int& lineTopIncludingMargins, int& lineBottomIncludingMargins, bool& hasAnnotationsBefore, bool& hasAnnotationsAfter, FontBaseline baselineType)
{
    if (isRootInlineBox())
        setLogicalTop(top + maxAscent - baselinePosition(baselineType)); // Place our root box.

    for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
        if (curr->renderer()->isPositioned())
            continue; // Positioned placeholders don't affect calculations.
        
        bool isInlineFlow = curr->isInlineFlowBox();
        if (isInlineFlow)
            static_cast<InlineFlowBox*>(curr)->placeBoxesInBlockDirection(top, maxHeight, maxAscent, strictMode, lineTop, lineBottom, setLineTop,
                                                                          lineTopIncludingMargins, lineBottomIncludingMargins, hasAnnotationsBefore, hasAnnotationsAfter, baselineType);

        bool childAffectsTopBottomPos = true;
        if (curr->verticalAlign() == TOP)
            curr->setLogicalTop(top);
        else if (curr->verticalAlign() == BOTTOM)
            curr->setLogicalTop(top + maxHeight - curr->lineHeight());
        else {
            if ((isInlineFlow && !static_cast<InlineFlowBox*>(curr)->hasTextChildren()) && !curr->boxModelObject()->hasInlineDirectionBordersOrPadding() && !strictMode)
                childAffectsTopBottomPos = false;
            int posAdjust = maxAscent - curr->baselinePosition(baselineType);
            curr->setLogicalTop(curr->logicalTop() + top + posAdjust);
        }
        
        int newLogicalTop = curr->logicalTop();
        int newLogicalTopIncludingMargins = newLogicalTop;
        int boxHeight = curr->logicalHeight();
        int boxHeightIncludingMargins = boxHeight;
            
        if (curr->isText() || curr->isInlineFlowBox()) {
            const FontMetrics& fontMetrics = curr->renderer()->style(m_firstLine)->fontMetrics();
            newLogicalTop += curr->baselinePosition(baselineType) - fontMetrics.ascent(baselineType);
            if (curr->isInlineFlowBox()) {
                RenderBoxModelObject* boxObject = toRenderBoxModelObject(curr->renderer());
                newLogicalTop -= boxObject->style(m_firstLine)->isHorizontalWritingMode() ? boxObject->borderTop() + boxObject->paddingTop() : 
                                 boxObject->borderRight() + boxObject->paddingRight();
            }
            newLogicalTopIncludingMargins = newLogicalTop;
        } else if (!curr->renderer()->isBR()) {
            RenderBox* box = toRenderBox(curr->renderer());
            newLogicalTopIncludingMargins = newLogicalTop;
            int overSideMargin = curr->isHorizontal() ? box->marginTop() : box->marginRight();
            int underSideMargin = curr->isHorizontal() ? box->marginBottom() : box->marginLeft();
            newLogicalTop += overSideMargin;
            boxHeightIncludingMargins += overSideMargin + underSideMargin;
        }

        curr->setLogicalTop(newLogicalTop);

        if (childAffectsTopBottomPos) {
            if (curr->renderer()->isRubyRun()) {
                if (!renderer()->style()->isFlippedLinesWritingMode())
                    hasAnnotationsBefore = true;
                else
                    hasAnnotationsAfter = true;

                RenderRubyRun* rubyRun = static_cast<RenderRubyRun*>(curr->renderer());
                if (RenderRubyBase* rubyBase = rubyRun->rubyBase()) {
                    int bottomRubyBaseLeading = (curr->logicalHeight() - rubyBase->logicalBottom()) + rubyBase->logicalHeight() - (rubyBase->lastRootBox() ? rubyBase->lastRootBox()->lineBottom() : 0);
                    int topRubyBaseLeading = rubyBase->logicalTop() + (rubyBase->firstRootBox() ? rubyBase->firstRootBox()->lineTop() : 0);
                    newLogicalTop += !renderer()->style()->isFlippedLinesWritingMode() ? topRubyBaseLeading : bottomRubyBaseLeading;
                    boxHeight -= (topRubyBaseLeading + bottomRubyBaseLeading);
                }
            }
            if (curr->isInlineTextBox()) {
                TextEmphasisPosition emphasisMarkPosition;
                if (static_cast<InlineTextBox*>(curr)->getEmphasisMarkPosition(curr->renderer()->style(m_firstLine), emphasisMarkPosition)) {
                    bool emphasisMarkIsOver = emphasisMarkPosition == TextEmphasisPositionOver;
                    if (emphasisMarkIsOver != curr->renderer()->style(m_firstLine)->isFlippedLinesWritingMode())
                        hasAnnotationsBefore = true;
                    else
                        hasAnnotationsAfter = true;
                }
            }

            if (!setLineTop) {
                setLineTop = true;
                lineTop = newLogicalTop;
                lineTopIncludingMargins = min(lineTop, newLogicalTopIncludingMargins);
            } else {
                lineTop = min(lineTop, newLogicalTop);
                lineTopIncludingMargins = min(lineTop, min(lineTopIncludingMargins, newLogicalTopIncludingMargins));
            }
            lineBottom = max(lineBottom, newLogicalTop + boxHeight);
            lineBottomIncludingMargins = max(lineBottom, max(lineBottomIncludingMargins, newLogicalTopIncludingMargins + boxHeightIncludingMargins));
        }
    }

    if (isRootInlineBox()) {
        const FontMetrics& fontMetrics = renderer()->style(m_firstLine)->fontMetrics();
        setLogicalTop(logicalTop() + baselinePosition(baselineType) - fontMetrics.ascent(baselineType));
        
        if (hasTextChildren() || strictMode) {
            if (!setLineTop) {
                setLineTop = true;
                lineTop = logicalTop();
                lineTopIncludingMargins = lineTop;
            } else {
                lineTop = min(lineTop, logicalTop());
                lineTopIncludingMargins = min(lineTop, lineTopIncludingMargins);
            }
            lineBottom = max(lineBottom, logicalTop() + logicalHeight());
            lineBottomIncludingMargins = max(lineBottom, lineBottomIncludingMargins);
        }
        
        if (renderer()->style()->isFlippedLinesWritingMode())
            flipLinesInBlockDirection(lineTopIncludingMargins, lineBottomIncludingMargins);
    }
}
