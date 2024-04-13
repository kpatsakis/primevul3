double AccessibilityUIElement::width()
{
    if (!m_element || !ATK_IS_OBJECT(m_element))
        return 0.0f;

    int width, height;
    atk_component_get_size(ATK_COMPONENT(m_element), &width, &height);
    return width;
}
