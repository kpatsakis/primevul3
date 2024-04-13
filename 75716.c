static PassRefPtr<CSSValue> valueForGridTrackBreadth(const Length& trackLength, const RenderStyle* style)
{
    if (trackLength.isPercent())
        return cssValuePool().createValue(trackLength);
    if (trackLength.isAuto())
        return cssValuePool().createIdentifierValue(CSSValueAuto);
    return zoomAdjustedPixelValue(trackLength.value(), style);
}
