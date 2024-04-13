bool AccessibilityUIElement::isEnabled()
{
    return checkElementState(m_element, ATK_STATE_ENABLED);
}
