bool RenderMenuList::itemIsSelected(unsigned listIndex) const
{
    const Vector<HTMLElement*>& listItems = toHTMLSelectElement(node())->listItems();
    if (listIndex >= listItems.size())
        return false;
    HTMLElement* element = listItems[listIndex];
    return element->hasTagName(optionTag) && toHTMLOptionElement(element)->selected();
}
