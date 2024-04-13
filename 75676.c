static PassRefPtr<CSSPrimitiveValue> fontVariantFromStyle(RenderStyle* style)
{
    if (style->fontDescription().smallCaps())
        return cssValuePool().createIdentifierValue(CSSValueSmallCaps);
    return cssValuePool().createIdentifierValue(CSSValueNormal);
}
