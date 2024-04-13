void RenderMenuList::adjustInnerStyle()
{
    RenderStyle* innerStyle = m_innerBlock->style();
    innerStyle->setBoxFlex(1);
    
    innerStyle->setPaddingLeft(Length(theme()->popupInternalPaddingLeft(style()), Fixed));
    innerStyle->setPaddingRight(Length(theme()->popupInternalPaddingRight(style()), Fixed));
    innerStyle->setPaddingTop(Length(theme()->popupInternalPaddingTop(style()), Fixed));
    innerStyle->setPaddingBottom(Length(theme()->popupInternalPaddingBottom(style()), Fixed));

    if (document()->page()->chrome()->selectItemWritingDirectionIsNatural()) {
        innerStyle->setTextAlign(LEFT);
        TextDirection direction = (m_buttonText && m_buttonText->text()->defaultWritingDirection() == WTF::Unicode::RightToLeft) ? RTL : LTR;
        innerStyle->setDirection(direction);
    } else if (m_optionStyle && document()->page()->chrome()->selectItemAlignmentFollowsMenuWritingDirection()) {
        if ((m_optionStyle->direction() != innerStyle->direction() || m_optionStyle->unicodeBidi() != innerStyle->unicodeBidi()))
            m_innerBlock->setNeedsLayoutAndPrefWidthsRecalc();
        innerStyle->setTextAlign(style()->isLeftToRightDirection() ? LEFT : RIGHT);
        innerStyle->setDirection(m_optionStyle->direction());
        innerStyle->setUnicodeBidi(m_optionStyle->unicodeBidi());
    }
}
