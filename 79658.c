HTMLDataListElement* HTMLInputElement::dataList() const
{
    if (!m_hasNonEmptyList)
        return 0;

    if (!m_inputType->shouldRespectListAttribute())
        return 0;

    Element* element = treeScope().getElementById(fastGetAttribute(listAttr));
    if (!element)
        return 0;
    if (!element->hasTagName(datalistTag))
        return 0;

    return toHTMLDataListElement(element);
}
