void RenderMenuList::styleDidChange(StyleDifference diff, const RenderStyle* oldStyle)
{
    RenderBlock::styleDidChange(diff, oldStyle);

    if (m_buttonText)
        m_buttonText->setStyle(style());
    if (m_innerBlock) // RenderBlock handled updating the anonymous block's style.
        adjustInnerStyle();

    bool fontChanged = !oldStyle || oldStyle->font() != style()->font();
    if (fontChanged)
        updateOptionsWidth();
}
