String RenderMenuList::itemText(unsigned listIndex) const
{
    HTMLSelectElement* select = toHTMLSelectElement(node());
    const Vector<HTMLElement*>& listItems = select->listItems();
    if (listIndex >= listItems.size())
        return String();

    String itemString;
    Element* element = listItems[listIndex];
    if (element->hasTagName(optgroupTag))
        itemString = static_cast<const HTMLOptGroupElement*>(element)->groupLabelText();
    else if (element->hasTagName(optionTag))
        itemString = toHTMLOptionElement(element)->textIndentedToRespectGroupLabel();

    applyTextTransform(style(), itemString, ' ');
    return itemString;
}
