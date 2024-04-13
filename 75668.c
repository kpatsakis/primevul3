String CSSComputedStyleDeclaration::cssText() const
{
    String result("");

    for (unsigned i = 0; i < numComputedProperties; i++) {
        if (i)
            result += " ";
        result += getPropertyName(computedProperties[i]);
        result += ": ";
        result += getPropertyValue(computedProperties[i]);
        result += ";";
    }

    return result;
}
