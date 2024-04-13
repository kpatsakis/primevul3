PopupMenuStyle RenderMenuList::menuStyle() const
{
    RenderStyle* s = m_innerBlock ? m_innerBlock->style() : style();
    return PopupMenuStyle(s->visitedDependentColor(CSSPropertyColor), s->visitedDependentColor(CSSPropertyBackgroundColor), s->font(), s->visibility() == VISIBLE, s->display() == NONE, s->textIndent(), style()->direction(), style()->unicodeBidi() == Override);
}
