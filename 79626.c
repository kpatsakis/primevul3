bool HTMLFormControlElement::recalcWillValidate() const
{
    if (m_dataListAncestorState == Unknown) {
        for (ContainerNode* ancestor = parentNode(); ancestor; ancestor = ancestor->parentNode()) {
            if (ancestor->hasTagName(datalistTag)) {
                m_dataListAncestorState = InsideDataList;
                break;
            }
        }
        if (m_dataListAncestorState == Unknown)
            m_dataListAncestorState = NotInsideDataList;
    }
    return m_dataListAncestorState == NotInsideDataList && !isDisabledOrReadOnly();
}
