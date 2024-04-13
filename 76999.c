bool AccessibilityUIElement::isSelected() const
{
    return checkElementState(m_element, ATK_STATE_SELECTED);
}
