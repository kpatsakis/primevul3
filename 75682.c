PassRefPtr<CSSValueList> CSSComputedStyleDeclaration::getCSSPropertyValuesForSidesShorthand(const StylePropertyShorthand& shorthand) const
{
    RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    RefPtr<CSSValue> topValue = getPropertyCSSValue(shorthand.properties()[0], DoNotUpdateLayout);
    RefPtr<CSSValue> rightValue = getPropertyCSSValue(shorthand.properties()[1], DoNotUpdateLayout);
    RefPtr<CSSValue> bottomValue = getPropertyCSSValue(shorthand.properties()[2], DoNotUpdateLayout);
    RefPtr<CSSValue> leftValue = getPropertyCSSValue(shorthand.properties()[3], DoNotUpdateLayout);

    if (!topValue || !rightValue || !bottomValue || !leftValue)
        return 0;

    bool showLeft = rightValue->cssText() != leftValue->cssText();
    bool showBottom = (topValue->cssText() != bottomValue->cssText()) || showLeft;
    bool showRight = (topValue->cssText() != rightValue->cssText()) || showBottom;

    list->append(topValue);
    if (showRight)
        list->append(rightValue);
    if (showBottom)
        list->append(bottomValue);
    if (showLeft)
        list->append(leftValue);

    return list.release();
}
