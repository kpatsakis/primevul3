int RenderMenuList::selectedIndex() const
{
    HTMLSelectElement* select = toHTMLSelectElement(node());
    return select->optionToListIndex(select->selectedIndex());
}
