int Element::offsetHeight()
{
    document()->updateLayoutIgnorePendingStylesheets();
    if (RenderBoxModelObject* renderer = renderBoxModelObject())
        return adjustLayoutUnitForAbsoluteZoom(renderer->pixelSnappedOffsetHeight(), renderer).round();
    return 0;
}
