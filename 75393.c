PopupMenuStyle RenderMenuList::itemStyle(unsigned listIndex) const
{
    const Vector<HTMLElement*>& listItems = toHTMLSelectElement(node())->listItems();
    if (listIndex >= listItems.size()) {
        if (!listIndex)
            return menuStyle();

        listIndex = 0;
    }
    HTMLElement* element = listItems[listIndex];
    
    RenderStyle* style = element->renderStyle() ? element->renderStyle() : element->computedStyle();
    return style ? PopupMenuStyle(style->visitedDependentColor(CSSPropertyColor), itemBackgroundColor(listIndex), style->font(), style->visibility() == VISIBLE, style->display() == NONE, style->textIndent(), style->direction(), style->unicodeBidi() == Override) : menuStyle();
}
