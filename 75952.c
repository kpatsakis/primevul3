LayoutUnit RenderBox::clientHeight() const
{
    return height() - borderTop() - borderBottom() - horizontalScrollbarHeight();
}
