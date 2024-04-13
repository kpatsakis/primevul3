static PassRefPtr<CSSPrimitiveValue> fontWeightFromStyle(RenderStyle* style)
{
    switch (style->fontDescription().weight()) {
    case FontWeight100:
        return cssValuePool().createIdentifierValue(CSSValue100);
    case FontWeight200:
        return cssValuePool().createIdentifierValue(CSSValue200);
    case FontWeight300:
        return cssValuePool().createIdentifierValue(CSSValue300);
    case FontWeightNormal:
        return cssValuePool().createIdentifierValue(CSSValueNormal);
    case FontWeight500:
        return cssValuePool().createIdentifierValue(CSSValue500);
    case FontWeight600:
        return cssValuePool().createIdentifierValue(CSSValue600);
    case FontWeightBold:
        return cssValuePool().createIdentifierValue(CSSValueBold);
    case FontWeight800:
        return cssValuePool().createIdentifierValue(CSSValue800);
    case FontWeight900:
        return cssValuePool().createIdentifierValue(CSSValue900);
    }
    ASSERT_NOT_REACHED();
    return cssValuePool().createIdentifierValue(CSSValueNormal);
}
