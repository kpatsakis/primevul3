void Element::updateLabel(TreeScope* scope, const AtomicString& oldForAttributeValue, const AtomicString& newForAttributeValue)
{
    ASSERT(hasTagName(labelTag));

    if (!inDocument())
        return;

    if (oldForAttributeValue == newForAttributeValue)
        return;

    if (!oldForAttributeValue.isEmpty())
        scope->removeLabel(oldForAttributeValue, static_cast<HTMLLabelElement*>(this));
    if (!newForAttributeValue.isEmpty())
        scope->addLabel(newForAttributeValue, static_cast<HTMLLabelElement*>(this));
}
