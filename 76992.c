bool AccessibilityUIElement::isFocusable() const
{
    return checkElementState(m_element, ATK_STATE_FOCUSABLE);
}
