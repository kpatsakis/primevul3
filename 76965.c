int AccessibilityUIElement::columnCount()
{
    if (!m_element || !ATK_IS_TABLE(m_element))
        return 0;

    return atk_table_get_n_columns(ATK_TABLE(m_element));
}
