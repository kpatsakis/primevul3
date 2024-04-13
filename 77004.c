double AccessibilityUIElement::maxValue()
{
    if (!m_element || !ATK_IS_OBJECT(m_element))
        return 0.0f;

    GValue value = G_VALUE_INIT;
    atk_value_get_maximum_value(ATK_VALUE(m_element), &value);
    if (!G_VALUE_HOLDS_FLOAT(&value))
        return 0.0f;

    return g_value_get_float(&value);
}
