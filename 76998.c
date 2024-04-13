bool AccessibilityUIElement::isSelectable() const
{
    return checkElementState(m_element, ATK_STATE_SELECTABLE);
}
