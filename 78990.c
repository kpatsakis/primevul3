void Element::setScrollTop(int newTop)
{
    document()->updateLayoutIgnorePendingStylesheets();
    if (RenderBox* rend = renderBox())
        rend->setScrollTop(static_cast<int>(newTop * rend->style()->effectiveZoom()));
}
