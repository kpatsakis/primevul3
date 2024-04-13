unsigned AccessibilityUIElement::indexOfChild(AccessibilityUIElement* element)
{
    if (!m_element || !ATK_IS_OBJECT(m_element))
        return 0;

    Vector<RefPtr<AccessibilityUIElement> > children;
    getChildren(children);

    unsigned count = children.size();
    for (unsigned i = 0; i < count; i++)
        if (children[i]->isEqual(element))
            return i;

    return 0;
}
