void Element::attributeChanged(const QualifiedName& name, const AtomicString& newValue, AttributeModificationReason)
{
    if (ElementShadow* parentElementShadow = shadowOfParentForDistribution(this)) {
        if (shouldInvalidateDistributionWhenAttributeChanged(parentElementShadow, name, newValue))
            parentElementShadow->invalidateDistribution();
    }

    parseAttribute(name, newValue);

    document()->incDOMTreeVersion();

    StyleResolver* styleResolver = document()->styleResolverIfExists();
    bool testShouldInvalidateStyle = attached() && styleResolver && styleChangeType() < FullStyleChange;
    bool shouldInvalidateStyle = false;

    if (isIdAttributeName(name)) {
        AtomicString oldId = elementData()->idForStyleResolution();
        AtomicString newId = makeIdForStyleResolution(newValue, document()->inQuirksMode());
        if (newId != oldId) {
            elementData()->setIdForStyleResolution(newId);
            shouldInvalidateStyle = testShouldInvalidateStyle && checkNeedsStyleInvalidationForIdChange(oldId, newId, styleResolver);
        }
    } else if (name == classAttr)
        classAttributeChanged(newValue);
    else if (name == HTMLNames::nameAttr)
        setHasName(!newValue.isNull());
    else if (name == HTMLNames::pseudoAttr)
        shouldInvalidateStyle |= testShouldInvalidateStyle && isInShadowTree();

    invalidateNodeListCachesInAncestors(&name, this);

    shouldInvalidateStyle |= !styleResolver;

    if (shouldInvalidateStyle)
        setNeedsStyleRecalc();

    if (AXObjectCache* cache = document()->existingAXObjectCache())
        cache->handleAttributeChanged(name, this);
}
