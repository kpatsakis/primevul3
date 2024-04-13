bool AccessibilityUIElement::isFocused() const
{
    return checkElementState(m_element, ATK_STATE_FOCUSED);
}
