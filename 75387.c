Color RenderMenuList::itemBackgroundColor(unsigned listIndex) const
{
    const Vector<HTMLElement*>& listItems = toHTMLSelectElement(node())->listItems();
    if (listIndex >= listItems.size())
        return style()->visitedDependentColor(CSSPropertyBackgroundColor);
    HTMLElement* element = listItems[listIndex];

    Color backgroundColor;
    if (element->renderStyle())
        backgroundColor = element->renderStyle()->visitedDependentColor(CSSPropertyBackgroundColor);
    if (!backgroundColor.hasAlpha())
        return backgroundColor;

    backgroundColor = style()->visitedDependentColor(CSSPropertyBackgroundColor).blend(backgroundColor);
    if (!backgroundColor.hasAlpha())
        return backgroundColor;

    return Color(Color::white).blend(backgroundColor);
}
