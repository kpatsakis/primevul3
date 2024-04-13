inline void Element::attributeChangedFromParserOrByCloning(const QualifiedName& name, const AtomicString& newValue, AttributeModificationReason reason)
{
    if (RuntimeEnabledFeatures::customDOMElementsEnabled() && name == isAttr) {
        document()->ensureCustomElementRegistry()->didGiveTypeExtension(this, newValue);
    }
    attributeChanged(name, newValue, reason);
}
