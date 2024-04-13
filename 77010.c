PassRefPtr<AccessibilityUIElement> AccessibilityUIElement::parentElement()
{
    if (!m_element || !ATK_IS_OBJECT(m_element))
        return 0;

    AtkObject* parent = atk_object_get_parent(ATK_OBJECT(m_element));
    return parent ? AccessibilityUIElement::create(parent) : 0;
}
