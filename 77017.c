JSRetainPtr<JSStringRef> AccessibilityUIElement::role()
{
    if (!m_element || !ATK_IS_OBJECT(m_element))
        return JSStringCreateWithCharacters(0, 0);

    AtkRole role = atk_object_get_role(ATK_OBJECT(m_element));
    if (!role)
        return JSStringCreateWithCharacters(0, 0);

    const gchar* roleName = atk_role_get_name(role);
    GOwnPtr<gchar> axRole(g_strdup_printf("AXRole: %s", roleName));

    return JSStringCreateWithUTF8CString(axRole.get());
}
