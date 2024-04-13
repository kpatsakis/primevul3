static inline Eina_Bool _ewk_frame_handle_key_scrolling(WebCore::Frame* frame, const WebCore::PlatformKeyboardEvent& keyEvent)
{
    WebCore::ScrollDirection direction;
    WebCore::ScrollGranularity granularity;

    int keyCode = keyEvent.windowsVirtualKeyCode();

    switch (keyCode) {
    case VK_SPACE:
        granularity = WebCore::ScrollByPage;
        if (keyEvent.shiftKey())
            direction = WebCore::ScrollUp;
        else
            direction = WebCore::ScrollDown;
        break;
    case VK_NEXT:
        granularity = WebCore::ScrollByPage;
        direction = WebCore::ScrollDown;
        break;
    case VK_PRIOR:
        granularity = WebCore::ScrollByPage;
        direction = WebCore::ScrollUp;
        break;
    case VK_HOME:
        granularity = WebCore::ScrollByDocument;
        direction = WebCore::ScrollUp;
        break;
    case VK_END:
        granularity = WebCore::ScrollByDocument;
        direction = WebCore::ScrollDown;
        break;
    case VK_LEFT:
        granularity = WebCore::ScrollByLine;
        direction = WebCore::ScrollLeft;
        break;
    case VK_RIGHT:
        granularity = WebCore::ScrollByLine;
        direction = WebCore::ScrollRight;
        break;
    case VK_UP:
        direction = WebCore::ScrollUp;
        if (keyEvent.ctrlKey())
            granularity = WebCore::ScrollByDocument;
        else
            granularity = WebCore::ScrollByLine;
        break;
    case VK_DOWN:
        direction = WebCore::ScrollDown;
        if (keyEvent.ctrlKey())
            granularity = WebCore::ScrollByDocument;
        else
            granularity = WebCore::ScrollByLine;
        break;
    default:
        return false;
    }

    if (frame->eventHandler()->scrollOverflow(direction, granularity))
        return false;

    frame->view()->scroll(direction, granularity);
    return true;
}
