static PassRefPtr<CSSPrimitiveValue> valueForFamily(const AtomicString& family)
{
    if (int familyIdentifier = identifierForFamily(family))
        return cssValuePool().createIdentifierValue(familyIdentifier);
    return cssValuePool().createValue(family.string(), CSSPrimitiveValue::CSS_STRING);
}
