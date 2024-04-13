JSRetainPtr<JSStringRef> AccessibilityUIElement::documentEncoding()
{
    if (!m_element || !ATK_IS_OBJECT(m_element))
        return JSStringCreateWithCharacters(0, 0);

    AtkRole role = atk_object_get_role(ATK_OBJECT(m_element));
    if (role != ATK_ROLE_DOCUMENT_FRAME)
        return JSStringCreateWithCharacters(0, 0);

    return JSStringCreateWithUTF8CString(atk_document_get_attribute_value(ATK_DOCUMENT(m_element), "Encoding"));
}
