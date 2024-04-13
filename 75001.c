void InlineFlowBox::computeLogicalBoxHeights(int& maxPositionTop, int& maxPositionBottom,
                                             int& maxAscent, int& maxDescent, bool& setMaxAscent, bool& setMaxDescent,
                                             bool strictMode, GlyphOverflowAndFallbackFontsMap& textBoxDataMap,
                                             FontBaseline baselineType, VerticalPositionCache& verticalPositionCache)
{
    if (isRootInlineBox()) {
        int height = lineHeight();
        int baseline = baselinePosition(baselineType);
        if (hasTextChildren() || strictMode) {
            int ascent = baseline;
            int descent = height - ascent;
            if (maxAscent < ascent || !setMaxAscent) {
                maxAscent = ascent;
                setMaxAscent = true;
            }
            if (maxDescent < descent || !setMaxDescent) {
                maxDescent = descent;
                setMaxDescent = true;
            }
        }
    }

    for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
        if (curr->renderer()->isPositioned())
            continue; // Positioned placeholders don't affect calculations.
        
        bool isInlineFlow = curr->isInlineFlowBox();
        
        bool affectsAscent = false;
        bool affectsDescent = false;
        
        curr->setLogicalTop(verticalPositionForBox(curr, baselineType, m_firstLine, verticalPositionCache));
        
        int lineHeight;
        int baseline;
        Vector<const SimpleFontData*>* usedFonts = 0;
        if (curr->isInlineTextBox()) {
            GlyphOverflowAndFallbackFontsMap::iterator it = textBoxDataMap.find(static_cast<InlineTextBox*>(curr));
            usedFonts = it == textBoxDataMap.end() ? 0 : &it->second.first;
        }

        if (usedFonts && !usedFonts->isEmpty() && curr->renderer()->style(m_firstLine)->lineHeight().isNegative()) {
            usedFonts->append(curr->renderer()->style(m_firstLine)->font().primaryFont());
            bool baselineSet = false;
            baseline = 0;
            int baselineToBottom = 0;
            for (size_t i = 0; i < usedFonts->size(); ++i) {
                const FontMetrics& fontMetrics = usedFonts->at(i)->fontMetrics();
                int halfLeading = (fontMetrics.lineSpacing() - fontMetrics.height()) / 2;
                int usedFontBaseline = halfLeading + fontMetrics.ascent(baselineType);
                int usedFontBaselineToBottom = fontMetrics.lineSpacing() - usedFontBaseline;
                if (!baselineSet) {
                    baselineSet = true;
                    baseline = usedFontBaseline;
                    baselineToBottom = usedFontBaselineToBottom;
                } else {
                    baseline = max(baseline, usedFontBaseline);
                    baselineToBottom = max(baselineToBottom, usedFontBaselineToBottom);
                }
                if (!affectsAscent)
                    affectsAscent = fontMetrics.ascent() - curr->logicalTop() > 0;
                if (!affectsDescent)
                    affectsDescent = fontMetrics.descent() + curr->logicalTop() > 0;
            }
            lineHeight = baseline + baselineToBottom;
        } else {
            lineHeight = curr->lineHeight();
            baseline = curr->baselinePosition(baselineType);
            if (curr->isText() || isInlineFlow) {
                const FontMetrics& fontMetrics = curr->renderer()->style(m_firstLine)->fontMetrics();
                affectsAscent = fontMetrics.ascent(baselineType) - curr->logicalTop() > 0;
                
                affectsDescent = fontMetrics.descent(baselineType) + curr->logicalTop() > 0;
            } else {
                affectsAscent = true;
                affectsDescent = true;
            }
        }

        if (curr->verticalAlign() == TOP) {
            if (maxPositionTop < lineHeight)
                maxPositionTop = lineHeight;
        } else if (curr->verticalAlign() == BOTTOM) {
            if (maxPositionBottom < lineHeight)
                maxPositionBottom = lineHeight;
        } else if ((!isInlineFlow || static_cast<InlineFlowBox*>(curr)->hasTextChildren()) || curr->boxModelObject()->hasInlineDirectionBordersOrPadding() || strictMode) {
            int ascent = baseline - curr->logicalTop();
            int descent = lineHeight - ascent;
            if (affectsAscent && (maxAscent < ascent || !setMaxAscent)) {
                maxAscent = ascent;
                setMaxAscent = true;
            }
            if (affectsDescent && (maxDescent < descent || !setMaxDescent)) {
                maxDescent = descent;
                setMaxDescent = true;
            }
        }

        if (curr->isInlineFlowBox())
            static_cast<InlineFlowBox*>(curr)->computeLogicalBoxHeights(maxPositionTop, maxPositionBottom, maxAscent, maxDescent,
                                                                        setMaxAscent, setMaxDescent, strictMode, textBoxDataMap,
                                                                        baselineType, verticalPositionCache);
    }
}
