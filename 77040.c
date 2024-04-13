JSRetainPtr<JSStringRef> AccessibilityUIElement::title()
{
    if (!m_element || !ATK_IS_OBJECT(m_element))
        return JSStringCreateWithCharacters(0, 0);

    const gchar* name = atk_object_get_name(ATK_OBJECT(m_element));
    if (!name)
        return JSStringCreateWithCharacters(0, 0);

    GOwnPtr<gchar> axTitle(g_strdup_printf("AXTitle: %s", name));

    return JSStringCreateWithUTF8CString(axTitle.get());
}
