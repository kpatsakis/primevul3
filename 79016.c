inline void Element::updateId(TreeScope* scope, const AtomicString& oldId, const AtomicString& newId)
{
    ASSERT(isInTreeScope());
    ASSERT(oldId != newId);

    if (!oldId.isEmpty())
        scope->removeElementById(oldId, this);
    if (!newId.isEmpty())
        scope->addElementById(newId, this);

    if (shouldRegisterAsExtraNamedItem())
        updateExtraNamedItemRegistration(oldId, newId);
}
