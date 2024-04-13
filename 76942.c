JSRetainPtr<JSStringRef> AccessibilityUIElement::allAttributes()
{
    if (!m_element || !ATK_IS_OBJECT(m_element))
        return JSStringCreateWithCharacters(0, 0);

    GOwnPtr<gchar> attributeData(attributeSetToString(atk_object_get_attributes(ATK_OBJECT(m_element))));
    return JSStringCreateWithUTF8CString(attributeData.get());
}
