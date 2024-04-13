void Element::scrollByUnits(int units, ScrollGranularity granularity)
{
    document()->updateLayoutIgnorePendingStylesheets();

    if (!renderer())
        return;

    if (!renderer()->hasOverflowClip())
        return;

    ScrollDirection direction = ScrollDown;
    if (units < 0) {
        direction = ScrollUp;
        units = -units;
    }
    Node* stopNode = this;
    toRenderBox(renderer())->scroll(direction, granularity, units, &stopNode);
}
