void AccessibilityUIElement::getChildren(Vector<RefPtr<AccessibilityUIElement> >& children)
{
    if (!m_element || !ATK_IS_OBJECT(m_element))
        return;

    int count = childrenCount();
    for (int i = 0; i < count; i++) {
        AtkObject* child = atk_object_ref_accessible_child(ATK_OBJECT(m_element), i);
        children.append(AccessibilityUIElement::create(child));
    }
}
