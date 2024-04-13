static PassRefPtr<CSSValueList> getBorderRadiusShorthandValue(const RenderStyle* style, RenderView* renderView)
{
    RefPtr<CSSValueList> list = CSSValueList::createSlashSeparated();
    bool showHorizontalBottomLeft = style->borderTopRightRadius().width() != style->borderBottomLeftRadius().width();
    bool showHorizontalBottomRight = style->borderBottomRightRadius().width() != style->borderTopLeftRadius().width();
    bool showHorizontalTopRight = style->borderTopRightRadius().width() != style->borderTopLeftRadius().width();

    bool showVerticalBottomLeft = style->borderTopRightRadius().height() != style->borderBottomLeftRadius().height();
    bool showVerticalBottomRight = (style->borderBottomRightRadius().height() != style->borderTopLeftRadius().height()) || showVerticalBottomLeft;
    bool showVerticalTopRight = (style->borderTopRightRadius().height() != style->borderTopLeftRadius().height()) || showVerticalBottomRight;
    bool showVerticalTopLeft = (style->borderTopLeftRadius().width() != style->borderTopLeftRadius().height());

    RefPtr<CSSValueList> topLeftRadius = getBorderRadiusCornerValues(style->borderTopLeftRadius(), style, renderView);
    RefPtr<CSSValueList> topRightRadius = getBorderRadiusCornerValues(style->borderTopRightRadius(), style, renderView);
    RefPtr<CSSValueList> bottomRightRadius = getBorderRadiusCornerValues(style->borderBottomRightRadius(), style, renderView);
    RefPtr<CSSValueList> bottomLeftRadius = getBorderRadiusCornerValues(style->borderBottomLeftRadius(), style, renderView);

    RefPtr<CSSValueList> horizontalRadii = CSSValueList::createSpaceSeparated();
    horizontalRadii->append(topLeftRadius->item(0));
    if (showHorizontalTopRight)
        horizontalRadii->append(topRightRadius->item(0));
    if (showHorizontalBottomRight)
        horizontalRadii->append(bottomRightRadius->item(0));
    if (showHorizontalBottomLeft)
        horizontalRadii->append(bottomLeftRadius->item(0));

    list->append(horizontalRadii);

    if (showVerticalTopLeft) {
        RefPtr<CSSValueList> verticalRadii = CSSValueList::createSpaceSeparated();
        verticalRadii->append(topLeftRadius->item(1));
        if (showVerticalTopRight)
            verticalRadii->append(topRightRadius->item(1));
        if (showVerticalBottomRight)
            verticalRadii->append(bottomRightRadius->item(1));
        if (showVerticalBottomLeft)
            verticalRadii->append(bottomLeftRadius->item(1));
        list->append(verticalRadii);
    }
    return list.release();
}
