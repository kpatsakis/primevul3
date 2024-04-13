bool AccessibilityUIElement::isVisible() const
{
    return checkElementState(m_element, ATK_STATE_VISIBLE);
}
