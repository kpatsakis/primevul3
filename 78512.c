RootInlineBox* RenderBlock::createLineBoxesFromBidiRuns(unsigned bidiLevel, BidiRunList<BidiRun>& bidiRuns, const InlineIterator& end, LineInfo& lineInfo, VerticalPositionCache& verticalPositionCache, BidiRun* trailingSpaceRun, WordMeasurements& wordMeasurements)
{
    if (!bidiRuns.runCount())
        return 0;

    lineInfo.setLastLine(!end.m_obj);

    RootInlineBox* lineBox = constructLine(bidiRuns, lineInfo);
    if (!lineBox)
        return 0;

    lineBox->setBidiLevel(bidiLevel);
    lineBox->setEndsWithBreak(lineInfo.previousLineBrokeCleanly());

    bool isSVGRootInlineBox = lineBox->isSVGRootInlineBox();

    GlyphOverflowAndFallbackFontsMap textBoxDataMap;

    if (!isSVGRootInlineBox)
        computeInlineDirectionPositionsForLine(lineBox, lineInfo, bidiRuns.firstRun(), trailingSpaceRun, end.atEnd(), textBoxDataMap, verticalPositionCache, wordMeasurements);

    computeBlockDirectionPositionsForLine(lineBox, bidiRuns.firstRun(), textBoxDataMap, verticalPositionCache);

    if (isSVGRootInlineBox) {
        ASSERT(isSVGText());
        static_cast<SVGRootInlineBox*>(lineBox)->computePerCharacterLayoutInformation();
    }

    lineBox->computeOverflow(lineBox->lineTop(), lineBox->lineBottom(), textBoxDataMap);

    return lineBox;
}
