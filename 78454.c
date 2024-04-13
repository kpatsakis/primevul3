static WebGestureEvent fatTap(int x, int y)
{
    WebGestureEvent event;
    event.type = WebInputEvent::GestureTap;
    event.x = x;
    event.y = y;
    event.data.tap.width = 50;
    event.data.tap.height = 50;
    return event;
}
