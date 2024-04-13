static PassRefPtr<CSSValue> zoomAdjustedPixelValueForLength(const Length& length, const RenderStyle* style)
{
    if (length.isFixed())
        return zoomAdjustedPixelValue(length.value(), style);
    return cssValuePool().createValue(length);
}
