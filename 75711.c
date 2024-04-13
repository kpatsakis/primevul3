PassRefPtr<CSSValue> CSSComputedStyleDeclaration::valueForCustomFilterNumberParameter(const CustomFilterNumberParameter* numberParameter) const
{
    RefPtr<CSSValueList> numberParameterValue = CSSValueList::createSpaceSeparated();
    for (unsigned i = 0; i < numberParameter->size(); ++i)
        numberParameterValue->append(cssValuePool().createValue(numberParameter->valueAt(i), CSSPrimitiveValue::CSS_NUMBER));
    return numberParameterValue.release();
}
