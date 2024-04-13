int RenderMenuList::listSize() const
{
    return toHTMLSelectElement(node())->listItems().size();
}
