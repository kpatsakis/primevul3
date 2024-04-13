int Element::offsetWidth()
{
    document()->updateLayoutIgnorePendingStylesheets();
    if (RenderBoxModelObject* renderer = renderBoxModelObject())
        return adjustLayoutUnitForAbsoluteZoom(renderer->pixelSnappedOffsetWidth(), renderer).round();
    return 0;
}
