PassRefPtr<CSSValueList> CSSComputedStyleDeclaration::getCSSPropertyValuesForShorthandProperties(const StylePropertyShorthand& shorthand) const
{
    RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    for (size_t i = 0; i < shorthand.length(); ++i) {
        RefPtr<CSSValue> value = getPropertyCSSValue(shorthand.properties()[i], DoNotUpdateLayout);
        list->append(value);
    }
    return list.release();
}
