int Element::clientWidth()
{
    document()->updateLayoutIgnorePendingStylesheets();

    bool inQuirksMode = document()->inQuirksMode();
    if ((!inQuirksMode && document()->documentElement() == this) ||
        (inQuirksMode && isHTMLElement() && document()->body() == this)) {
        if (FrameView* view = document()->view()) {
            if (RenderView* renderView = document()->renderView())
                return adjustForAbsoluteZoom(view->layoutWidth(), renderView);
        }
    }
    
    if (RenderBox* renderer = renderBox())
        return adjustLayoutUnitForAbsoluteZoom(renderer->pixelSnappedClientWidth(), renderer).round();
    return 0;
}
