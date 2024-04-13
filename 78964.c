int Element::scrollLeft()
{
    document()->updateLayoutIgnorePendingStylesheets();
    if (RenderBox* rend = renderBox())
        return adjustForAbsoluteZoom(rend->scrollLeft(), rend);
    return 0;
}
