static bool _ewk_frame_rect_cmp_less_than(const WebCore::IntRect& begin, const WebCore::IntRect& end)
{
    return (begin.y() < end.y() || (begin.y() == end.y() && begin.x() < end.x()));
}
