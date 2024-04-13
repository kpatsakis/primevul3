String RenderMenuList::itemAccessibilityText(unsigned listIndex) const
{
    const Vector<HTMLElement*>& listItems = toHTMLSelectElement(node())->listItems();
    if (listIndex >= listItems.size())
        return String();
    return listItems[listIndex]->fastGetAttribute(aria_labelAttr);
}
