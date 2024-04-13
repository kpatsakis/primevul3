int RenderBox::verticalScrollbarWidth() const
{
    return includeVerticalScrollbarSize() ? layer()->verticalScrollbarWidth() : 0;
}
