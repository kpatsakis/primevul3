void HTMLFormElement::removeFormElement(FormAssociatedElement* e)
{
    unsigned index;
    for (index = 0; index < m_associatedElements.size(); ++index) {
        if (m_associatedElements[index] == e)
            break;
    }
    ASSERT_WITH_SECURITY_IMPLICATION(index < m_associatedElements.size());
    if (index < m_associatedElementsBeforeIndex)
        --m_associatedElementsBeforeIndex;
    if (index < m_associatedElementsAfterIndex)
        --m_associatedElementsAfterIndex;
    removeFromPastNamesMap(*toHTMLElement(e));
    removeFromVector(m_associatedElements, e);
}
