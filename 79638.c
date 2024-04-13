bool HTMLFormControlElement::shouldHaveFocusAppearance() const
{
    ASSERT(focused());
    return shouldShowFocusRingOnMouseFocus() || !m_wasFocusedByMouse;
}
