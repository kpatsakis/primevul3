bool Element::pseudoStyleCacheIsInvalid(const RenderStyle* currentStyle, RenderStyle* newStyle)
{
    ASSERT(currentStyle == renderStyle());
    ASSERT(renderer());

    if (!currentStyle)
        return false;

    const PseudoStyleCache* pseudoStyleCache = currentStyle->cachedPseudoStyles();
    if (!pseudoStyleCache)
        return false;

    size_t cacheSize = pseudoStyleCache->size();
    for (size_t i = 0; i < cacheSize; ++i) {
        RefPtr<RenderStyle> newPseudoStyle;
        PseudoId pseudoId = pseudoStyleCache->at(i)->styleType();
        if (pseudoId == FIRST_LINE || pseudoId == FIRST_LINE_INHERITED)
            newPseudoStyle = renderer()->uncachedFirstLineStyle(newStyle);
        else
            newPseudoStyle = renderer()->getUncachedPseudoStyle(PseudoStyleRequest(pseudoId), newStyle, newStyle);
        if (!newPseudoStyle)
            return true;
        if (*newPseudoStyle != *pseudoStyleCache->at(i)) {
            if (pseudoId < FIRST_INTERNAL_PSEUDOID)
                newStyle->setHasPseudoStyle(pseudoId);
            newStyle->addCachedPseudoStyle(newPseudoStyle);
            if (pseudoId == FIRST_LINE || pseudoId == FIRST_LINE_INHERITED) {
                renderer()->setNeedsLayoutAndPrefWidthsRecalc();
            }
            return true;
        }
    }
    return false;
}
