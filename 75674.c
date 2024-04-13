static PassRefPtr<CSSPrimitiveValue> fontSizeFromStyle(RenderStyle* style)
{
    return zoomAdjustedPixelValue(style->fontDescription().computedPixelSize(), style);
}
