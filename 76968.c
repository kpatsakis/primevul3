JSRetainPtr<JSStringRef> AccessibilityUIElement::description()
{
    if (!m_element || !ATK_IS_OBJECT(m_element))
        return JSStringCreateWithCharacters(0, 0);

    const gchar* description = atk_object_get_description(ATK_OBJECT(m_element));
    if (!description)
        return JSStringCreateWithCharacters(0, 0);

    GOwnPtr<gchar> axDesc(g_strdup_printf("AXDescription: %s", description));

    return JSStringCreateWithUTF8CString(axDesc.get());
}
