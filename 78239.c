unsigned HTMLFormElement::formElementIndexWithFormAttribute(Element* element, unsigned rangeStart, unsigned rangeEnd)
{
    if (m_associatedElements.isEmpty())
        return 0;

    ASSERT(rangeStart <= rangeEnd);

    if (rangeStart == rangeEnd)
        return rangeStart;

    unsigned left = rangeStart;
    unsigned right = rangeEnd - 1;
    unsigned short position;

    while (left != right) {
        unsigned middle = left + ((right - left) / 2);
        ASSERT(middle < m_associatedElementsBeforeIndex || middle >= m_associatedElementsAfterIndex);
        position = element->compareDocumentPosition(toHTMLElement(m_associatedElements[middle]));
        if (position & DOCUMENT_POSITION_FOLLOWING)
            right = middle;
        else
            left = middle + 1;
    }

    ASSERT(left < m_associatedElementsBeforeIndex || left >= m_associatedElementsAfterIndex);
    position = element->compareDocumentPosition(toHTMLElement(m_associatedElements[left]));
    if (position & DOCUMENT_POSITION_FOLLOWING)
        return left;
    return left + 1;
}
