static PassRefPtr<CSSValueList> fontFamilyFromStyle(RenderStyle* style)
{
    const FontFamily& firstFamily = style->fontDescription().family();
    RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
    for (const FontFamily* family = &firstFamily; family; family = family->next())
        list->append(valueForFamily(family->family()));
    return list.release();
}
