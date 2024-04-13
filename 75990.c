int RenderBox::horizontalScrollbarHeight() const
{
    return includeHorizontalScrollbarSize() ? layer()->horizontalScrollbarHeight() : 0;
}
