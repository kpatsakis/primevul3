String RenderMenuList::itemToolTip(unsigned listIndex) const
{
    const Vector<HTMLElement*>& listItems = toHTMLSelectElement(node())->listItems();
    if (listIndex >= listItems.size())
        return String();
    return listItems[listIndex]->title();
}
