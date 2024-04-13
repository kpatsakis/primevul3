bool Element::shouldInvalidateDistributionWhenAttributeChanged(ElementShadow* elementShadow, const QualifiedName& name, const AtomicString& newValue)
{
    ASSERT(elementShadow);
    const SelectRuleFeatureSet& featureSet = elementShadow->distributor().ensureSelectFeatureSet(elementShadow);

    if (isIdAttributeName(name)) {
        AtomicString oldId = elementData()->idForStyleResolution();
        AtomicString newId = makeIdForStyleResolution(newValue, document()->inQuirksMode());
        if (newId != oldId) {
            if (!oldId.isEmpty() && featureSet.hasSelectorForId(oldId))
                return true;
            if (!newId.isEmpty() && featureSet.hasSelectorForId(newId))
                return true;
        }
    }

    if (name == HTMLNames::classAttr) {
        const AtomicString& newClassString = newValue;
        if (classStringHasClassName(newClassString)) {
            const bool shouldFoldCase = document()->inQuirksMode();
            const SpaceSplitString& oldClasses = elementData()->classNames();
            const SpaceSplitString newClasses(newClassString, shouldFoldCase);
            if (checkSelectorForClassChange(oldClasses, newClasses, featureSet))
                return true;
        } else {
            const SpaceSplitString& oldClasses = elementData()->classNames();
            if (checkSelectorForClassChange(oldClasses, featureSet))
                return true;
        }
    }

    return featureSet.hasSelectorForAttribute(name.localName());
}
