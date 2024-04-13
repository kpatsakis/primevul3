void Element::willModifyAttribute(const QualifiedName& name, const AtomicString& oldValue, const AtomicString& newValue)
{
    if (isIdAttributeName(name))
        updateId(oldValue, newValue);
    else if (name == HTMLNames::nameAttr)
        updateName(oldValue, newValue);
    else if (name == HTMLNames::forAttr && hasTagName(labelTag)) {
        TreeScope* scope = treeScope();
        if (scope->shouldCacheLabelsByForAttribute())
            updateLabel(scope, oldValue, newValue);
    }

    if (oldValue != newValue) {
        if (attached() && document()->styleResolver() && document()->styleResolver()->hasSelectorForAttribute(name.localName()))
           setNeedsStyleRecalc();
    }

    if (OwnPtr<MutationObserverInterestGroup> recipients = MutationObserverInterestGroup::createForAttributesMutation(this, name))
        recipients->enqueueMutationRecord(MutationRecord::createAttributes(this, name, oldValue));

    InspectorInstrumentation::willModifyDOMAttr(document(), this, oldValue, newValue);
}
