int Element::scrollWidth()
{
    document()->updateLayoutIgnorePendingStylesheets();
    if (RenderBox* rend = renderBox())
        return adjustForAbsoluteZoom(rend->scrollWidth(), rend);
    return 0;
}
