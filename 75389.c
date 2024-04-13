bool RenderMenuList::itemIsEnabled(unsigned listIndex) const
{
    const Vector<HTMLElement*>& listItems = toHTMLSelectElement(node())->listItems();
    if (listIndex >= listItems.size())
        return false;
    HTMLElement* element = listItems[listIndex];
    if (!element->hasTagName(optionTag))
        return false;

    bool groupEnabled = true;
    if (Element* parentElement = element->parentElement()) {
        if (parentElement->hasTagName(optgroupTag))
            groupEnabled = !static_cast<HTMLOptGroupElement*>(parentElement)->disabled();
    }
    if (!groupEnabled)
        return false;

    return element->isEnabledFormControl();
}
