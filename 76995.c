bool AccessibilityUIElement::isMultiSelectable() const
{
    return checkElementState(m_element, ATK_STATE_MULTISELECTABLE);
}
