bool AccessibilityUIElement::isExpanded() const
{
    return checkElementState(m_element, ATK_STATE_EXPANDED);
}
