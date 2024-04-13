static PassRefPtr<CSSValue> fillSizeToCSSValue(const FillSize& fillSize, const RenderStyle* style)
{
    if (fillSize.type == Contain)
        return cssValuePool().createIdentifierValue(CSSValueContain);

    if (fillSize.type == Cover)
        return cssValuePool().createIdentifierValue(CSSValueCover);

    if (fillSize.size.height().isAuto())
        return zoomAdjustedPixelValueForLength(fillSize.size.width(), style);

    RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    list->append(zoomAdjustedPixelValueForLength(fillSize.size.width(), style));
    list->append(zoomAdjustedPixelValueForLength(fillSize.size.height(), style));
    return list.release();
}
