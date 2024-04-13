Node* HTMLFormElement::elementFromPastNamesMap(const AtomicString& pastName) const
{
    if (pastName.isEmpty() || !m_pastNamesMap)
        return 0;
    Node* node = m_pastNamesMap->get(pastName.impl());
#if !ASSERT_DISABLED
    if (!node)
        return 0;
    ASSERT_WITH_SECURITY_IMPLICATION(toHTMLElement(node)->form() == this);
    if (node->hasTagName(imgTag)) {
        ASSERT_WITH_SECURITY_IMPLICATION(m_imageElements.find(node) != kNotFound);
    } else if (node->hasTagName(objectTag)) {
        ASSERT_WITH_SECURITY_IMPLICATION(m_associatedElements.find(toHTMLObjectElement(node)) != kNotFound);
    } else {
        ASSERT_WITH_SECURITY_IMPLICATION(m_associatedElements.find(toHTMLFormControlElement(node)) != kNotFound);
    }
#endif
    return node;
}
