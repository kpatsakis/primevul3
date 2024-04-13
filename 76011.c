IntRect RenderBox::overflowClipRect(const IntPoint& location, OverlayScrollbarSizeRelevancy relevancy)
{
    IntRect clipRect(location + IntSize(borderLeft(), borderTop()),
        size() - IntSize(borderLeft() + borderRight(), borderTop() + borderBottom()));

    if (layer())
        clipRect.contract(layer()->verticalScrollbarWidth(relevancy), layer()->horizontalScrollbarHeight(relevancy));

    return clipRect;
}
