void RenderMenuList::didUpdateActiveOption(int optionIndex)
{
    if (!AXObjectCache::accessibilityEnabled())
        return;

    if (m_lastActiveIndex == optionIndex)
        return;
    m_lastActiveIndex = optionIndex;

    HTMLSelectElement* select = toHTMLSelectElement(node());
    int listIndex = select->optionToListIndex(optionIndex);
    if (listIndex < 0 || listIndex >= static_cast<int>(select->listItems().size()))
        return;

    ASSERT(select->listItems()[listIndex]);

    if (AccessibilityMenuList* menuList = static_cast<AccessibilityMenuList*>(document()->axObjectCache()->get(this)))
        menuList->didUpdateActiveOption(optionIndex);
}
