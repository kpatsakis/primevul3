PassRefPtr<StylePropertySet> CSSComputedStyleDeclaration::copyPropertiesInSet(const CSSPropertyID* set, unsigned length) const
{
    Vector<CSSProperty, 256> list;
    list.reserveInitialCapacity(length);
    for (unsigned i = 0; i < length; ++i) {
        RefPtr<CSSValue> value = getPropertyCSSValue(set[i]);
        if (value)
            list.append(CSSProperty(set[i], value.release(), false));
    }
    return StylePropertySet::create(list.data(), list.size());
}
