static PassRefPtr<CSSPrimitiveValue> lineHeightFromStyle(RenderStyle* style, RenderView* renderView)
{
    Length length = style->lineHeight();
    if (length.isNegative())
        return cssValuePool().createIdentifierValue(CSSValueNormal);
    if (length.isPercent())
        return zoomAdjustedPixelValue(static_cast<int>(length.percent() * style->fontDescription().specifiedSize()) / 100, style);
    return zoomAdjustedPixelValue(valueForLength(length, 0, renderView), style);
}
