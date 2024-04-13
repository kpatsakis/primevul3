double AccessibilityUIElement::x()
{
    if (!m_element || !ATK_IS_OBJECT(m_element))
        return 0.0f;

    int x, y;
    atk_component_get_position(ATK_COMPONENT(m_element), &x, &y, ATK_XY_SCREEN);
    return x;
}
