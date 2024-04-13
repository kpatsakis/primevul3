    static WebCore::FloatSize computeRelativeOffset(const WebCore::IntPoint& absoluteOffset, const WebCore::LayoutRect& rect)
    {
        WebCore::FloatSize relativeOffset = WebCore::FloatPoint(absoluteOffset) - rect.location();
        relativeOffset.scale(1.f / rect.width(), 1.f / rect.height());
        return relativeOffset;
    }
