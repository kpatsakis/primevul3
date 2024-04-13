bool RenderBox::canBeScrolledAndHasScrollableArea() const
{
    return canBeProgramaticallyScrolled() && (scrollHeight() != clientHeight() || scrollWidth() != clientWidth());
}
