unsigned HTMLFormElement::formElementIndex(FormAssociatedElement* associatedElement)
{
    HTMLElement* associatedHTMLElement = toHTMLElement(associatedElement);
    if (associatedHTMLElement->fastHasAttribute(formAttr)) {
        unsigned short position = compareDocumentPosition(associatedHTMLElement);
        if (position & DOCUMENT_POSITION_PRECEDING) {
            ++m_associatedElementsBeforeIndex;
            ++m_associatedElementsAfterIndex;
            return HTMLFormElement::formElementIndexWithFormAttribute(associatedHTMLElement, 0, m_associatedElementsBeforeIndex - 1);
        }
        if (position & DOCUMENT_POSITION_FOLLOWING && !(position & DOCUMENT_POSITION_CONTAINED_BY))
            return HTMLFormElement::formElementIndexWithFormAttribute(associatedHTMLElement, m_associatedElementsAfterIndex, m_associatedElements.size());
    }

    if (ElementTraversal::next(associatedHTMLElement, this)) {
        unsigned i = m_associatedElementsBeforeIndex;
        for (Element* element = this; element; element = ElementTraversal::next(element, this)) {
            if (element == associatedHTMLElement) {
                ++m_associatedElementsAfterIndex;
                return i;
            }
            if (!element->isFormControlElement() && !element->hasTagName(objectTag))
                continue;
            if (!element->isHTMLElement() || toHTMLElement(element)->form() != this)
                continue;
            ++i;
        }
    }
    return m_associatedElementsAfterIndex++;
}
