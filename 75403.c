void RenderMenuList::setTextFromItem(unsigned listIndex)
{
    setTextFromOption(toHTMLSelectElement(node())->listToOptionIndex(listIndex));
}
