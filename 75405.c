void RenderMenuList::showPopup()
{
    if (m_popupIsVisible)
        return;

    if (document()->page()->chrome()->hasOpenedPopup())
        return;

    createInnerBlock();
    if (!m_popup)
        m_popup = document()->page()->chrome()->createPopupMenu(this);
    m_popupIsVisible = true;

    FloatPoint absTopLeft = localToAbsolute(FloatPoint(), false, true);
    LayoutRect absBounds = absoluteBoundingBoxRectIgnoringTransforms();
    absBounds.setLocation(roundedLayoutPoint(absTopLeft));
    HTMLSelectElement* select = toHTMLSelectElement(node());
    m_popup->show(absBounds, document()->view(), select->optionToListIndex(select->selectedIndex()));
}
