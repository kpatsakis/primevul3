static PassRefPtr<CSSValue> renderTextDecorationFlagsToCSSValue(int textDecoration)
{
    RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    if (textDecoration & UNDERLINE)
        list->append(cssValuePool().createIdentifierValue(CSSValueUnderline));
    if (textDecoration & OVERLINE)
        list->append(cssValuePool().createIdentifierValue(CSSValueOverline));
    if (textDecoration & LINE_THROUGH)
        list->append(cssValuePool().createIdentifierValue(CSSValueLineThrough));
    if (textDecoration & BLINK)
        list->append(cssValuePool().createIdentifierValue(CSSValueBlink));

    if (!list->length())
        return cssValuePool().createIdentifierValue(CSSValueNone);
    return list;
}
