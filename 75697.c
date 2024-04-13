String CSSComputedStyleDeclaration::item(unsigned i) const
{
    if (i >= length())
        return "";

    return getPropertyName(computedProperties[i]);
}
