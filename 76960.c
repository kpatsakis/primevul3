PassRefPtr<AccessibilityUIElement> AccessibilityUIElement::cellForColumnAndRow(unsigned col, unsigned row)
{
    if (!m_element || !ATK_IS_TABLE(m_element))
        return 0;

    AtkObject* foundCell = atk_table_ref_at(ATK_TABLE(m_element), row, col);
    return foundCell ? AccessibilityUIElement::create(foundCell) : 0;
}
