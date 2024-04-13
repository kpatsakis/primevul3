static PassRefPtr<CSSValue> valueForGridPosition(const Length& position)
{
    if (position.isAuto())
        return cssValuePool().createIdentifierValue(CSSValueAuto);

    ASSERT(position.isFixed());
    return cssValuePool().createValue(position.value(), CSSPrimitiveValue::CSS_NUMBER);
}
