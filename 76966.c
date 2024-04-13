JSRetainPtr<JSStringRef> AccessibilityUIElement::columnIndexRange()
{
    return indexRangeInTable(m_element, false);
}
