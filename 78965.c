int Element::scrollTop()
{
    document()->updateLayoutIgnorePendingStylesheets();
    if (RenderBox* rend = renderBox())
        return adjustForAbsoluteZoom(rend->scrollTop(), rend);
    return 0;
}
