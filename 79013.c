void Element::updateExtraNamedItemRegistration(const AtomicString& oldId, const AtomicString& newId)
{
    if (!document()->isHTMLDocument())
        return;

    if (!oldId.isEmpty())
        toHTMLDocument(document())->removeExtraNamedItem(oldId);

    if (!newId.isEmpty())
        toHTMLDocument(document())->addExtraNamedItem(newId);
}
