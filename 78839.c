int Element::clientLeft()
{
    document()->updateLayoutIgnorePendingStylesheets();

    if (RenderBox* renderer = renderBox())
        return adjustForAbsoluteZoom(roundToInt(renderer->clientLeft()), renderer);
    return 0;
}
