int Element::clientHeight()
{
    document()->updateLayoutIgnorePendingStylesheets();

    bool inQuirksMode = document()->inQuirksMode();     

    if ((!inQuirksMode && document()->documentElement() == this) ||
        (inQuirksMode && isHTMLElement() && document()->body() == this)) {
        if (FrameView* view = document()->view()) {
            if (RenderView* renderView = document()->renderView())
                return adjustForAbsoluteZoom(view->layoutHeight(), renderView);
        }
    }
    
    if (RenderBox* renderer = renderBox())
        return adjustLayoutUnitForAbsoluteZoom(renderer->pixelSnappedClientHeight(), renderer).round();
    return 0;
}
