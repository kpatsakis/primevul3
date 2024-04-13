void HTMLFormElement::addToPastNamesMap(Node* element, const AtomicString& pastName)
{
    if (pastName.isEmpty())
        return;
    if (!m_pastNamesMap)
        m_pastNamesMap = adoptPtr(new PastNamesMap);
    m_pastNamesMap->set(pastName.impl(), element);
}
