PassRefPtr<CSSValue> CSSComputedStyleDeclaration::valueForCustomFilterParameter(const CustomFilterParameter* parameter) const
{
    ASSERT(parameter);
    switch (parameter->parameterType()) {
    case CustomFilterParameter::NUMBER:
        return valueForCustomFilterNumberParameter(static_cast<const CustomFilterNumberParameter*>(parameter));
    }
    
    ASSERT_NOT_REACHED();
    return 0;
}
