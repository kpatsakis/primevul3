int RenderMenuList::clientPaddingRight() const
{
    if (style()->appearance() == MenulistPart || style()->appearance() == MenulistButtonPart) {
        return endOfLinePadding;
    }

    return paddingRight() + m_innerBlock->paddingRight();
}
