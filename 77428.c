static bool _ewk_frame_rect_is_negative_value(const WebCore::IntRect& rect)
{
    return (rect.x() < 0 || rect.y() < 0);
}
