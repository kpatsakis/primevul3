void AccessibilityUIElement::getChildrenWithRange(Vector<RefPtr<AccessibilityUIElement> >& children, unsigned location, unsigned length)
{
    if (!m_element || !ATK_IS_OBJECT(m_element))
        return;
    unsigned end = location + length;
    for (unsigned i = location; i < end; i++) {
        AtkObject* child = atk_object_ref_accessible_child(ATK_OBJECT(m_element), i);
        children.append(AccessibilityUIElement::create(child));
    }
}
