static inline void setLogicalWidthForTextRun(RootInlineBox* lineBox, BidiRun* run, RenderText* renderer, float xPos, const LineInfo& lineInfo,
                                             GlyphOverflowAndFallbackFontsMap& textBoxDataMap, VerticalPositionCache& verticalPositionCache, WordMeasurements& wordMeasurements)
{
    HashSet<const SimpleFontData*> fallbackFonts;
    GlyphOverflow glyphOverflow;

    const Font& font = renderer->style(lineInfo.isFirstLine())->font();
    if (lineBox->fitsToGlyphs()) {
        bool includeRootLine = lineBox->includesRootLineBoxFontOrLeading();
        int baselineShift = lineBox->verticalPositionForBox(run->m_box, verticalPositionCache);
        int rootDescent = includeRootLine ? font.fontMetrics().descent() : 0;
        int rootAscent = includeRootLine ? font.fontMetrics().ascent() : 0;
        int boxAscent = font.fontMetrics().ascent() - baselineShift;
        int boxDescent = font.fontMetrics().descent() + baselineShift;
        if (boxAscent > rootDescent ||  boxDescent > rootAscent)
            glyphOverflow.computeBounds = true;
    }

    LayoutUnit hyphenWidth = 0;
    if (toInlineTextBox(run->m_box)->hasHyphen()) {
        const Font& font = renderer->style(lineInfo.isFirstLine())->font();
        hyphenWidth = measureHyphenWidth(renderer, font);
    }
    float measuredWidth = 0;

    bool kerningIsEnabled = font.typesettingFeatures() & Kerning;

#if OS(DARWIN)
    bool canUseSimpleFontCodePath = renderer->canUseSimpleFontCodePath() && !font.fontDescription().featureSettings();
#else
    bool canUseSimpleFontCodePath = renderer->canUseSimpleFontCodePath();
#endif


    if (!lineBox->fitsToGlyphs() && canUseSimpleFontCodePath) {
        int lastEndOffset = run->m_start;
        for (size_t i = 0, size = wordMeasurements.size(); i < size && lastEndOffset < run->m_stop; ++i) {
            const WordMeasurement& wordMeasurement = wordMeasurements[i];
            if (wordMeasurement.width <=0 || wordMeasurement.startOffset == wordMeasurement.endOffset)
                continue;
            if (wordMeasurement.renderer != renderer || wordMeasurement.startOffset != lastEndOffset || wordMeasurement.endOffset > run->m_stop)
                continue;

            lastEndOffset = wordMeasurement.endOffset;
            if (kerningIsEnabled && lastEndOffset == run->m_stop) {
                int wordLength = lastEndOffset - wordMeasurement.startOffset;
                measuredWidth += renderer->width(wordMeasurement.startOffset, wordLength, xPos, lineInfo.isFirstLine());
                if (i > 0 && wordLength == 1 && renderer->characterAt(wordMeasurement.startOffset) == ' ')
                    measuredWidth += renderer->style()->wordSpacing();
            } else
                measuredWidth += wordMeasurement.width;
            if (!wordMeasurement.fallbackFonts.isEmpty()) {
                HashSet<const SimpleFontData*>::const_iterator end = wordMeasurement.fallbackFonts.end();
                for (HashSet<const SimpleFontData*>::const_iterator it = wordMeasurement.fallbackFonts.begin(); it != end; ++it)
                    fallbackFonts.add(*it);
            }
        }
        if (measuredWidth && lastEndOffset != run->m_stop) {
            measuredWidth = 0;
            fallbackFonts.clear();
        }
    }

    if (!measuredWidth)
        measuredWidth = renderer->width(run->m_start, run->m_stop - run->m_start, xPos, lineInfo.isFirstLine(), &fallbackFonts, &glyphOverflow);

    run->m_box->setLogicalWidth(measuredWidth + hyphenWidth);
    if (!fallbackFonts.isEmpty()) {
        ASSERT(run->m_box->isText());
        GlyphOverflowAndFallbackFontsMap::iterator it = textBoxDataMap.add(toInlineTextBox(run->m_box), make_pair(Vector<const SimpleFontData*>(), GlyphOverflow())).iterator;
        ASSERT(it->value.first.isEmpty());
        copyToVector(fallbackFonts, it->value.first);
        run->m_box->parent()->clearDescendantsHaveSameLineHeightAndBaseline();
    }
    if ((glyphOverflow.top || glyphOverflow.bottom || glyphOverflow.left || glyphOverflow.right)) {
        ASSERT(run->m_box->isText());
        GlyphOverflowAndFallbackFontsMap::iterator it = textBoxDataMap.add(toInlineTextBox(run->m_box), make_pair(Vector<const SimpleFontData*>(), GlyphOverflow())).iterator;
        it->value.second = glyphOverflow;
        run->m_box->clearKnownToHaveNoOverflow();
    }
}
