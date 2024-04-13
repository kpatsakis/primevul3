void AccessibilityUIElement::press()
{
    if (!m_element || !ATK_IS_OBJECT(m_element))
        return;

    if (!ATK_IS_ACTION(m_element))
        return;

    atk_action_do_action(ATK_ACTION(m_element), 0);
}
