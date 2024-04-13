inline void Element::updateName(const AtomicString& oldName, const AtomicString& newName)
{
    if (!inDocument() || isInShadowTree())
        return;

    if (oldName == newName)
        return;

    if (shouldRegisterAsNamedItem())
        updateNamedItemRegistration(oldName, newName);
}
