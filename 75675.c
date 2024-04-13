static PassRefPtr<CSSPrimitiveValue> fontStyleFromStyle(RenderStyle* style)
{
    if (style->fontDescription().italic())
        return cssValuePool().createIdentifierValue(CSSValueItalic);
    return cssValuePool().createIdentifierValue(CSSValueNormal);
}
