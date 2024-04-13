RenderMenuList::RenderMenuList(Element* element)
    : RenderDeprecatedFlexibleBox(element)
    , m_buttonText(0)
    , m_innerBlock(0)
    , m_optionsChanged(true)
    , m_optionsWidth(0)
    , m_lastActiveIndex(-1)
    , m_popupIsVisible(false)
{
    ASSERT(element);
    ASSERT(element->isHTMLElement());
    ASSERT(element->hasTagName(HTMLNames::selectTag));
}
