Eina_Bool ewk_frame_feed_touch_event(Evas_Object* ewkFrame, Ewk_Touch_Event_Type action, Eina_List* points, int metaState)
{
#if ENABLE(TOUCH_EVENTS)
    EINA_SAFETY_ON_NULL_RETURN_VAL(points, false);
    EWK_FRAME_SD_GET(ewkFrame, smartData);

    if (!smartData || !smartData->frame || !ewk_view_need_touch_events_get(smartData->view))
        return false;

    Evas_Coord x, y;
    evas_object_geometry_get(smartData->view, &x, &y, 0, 0);

    WebCore::PlatformEvent::Type type;
    switch (action) {
    case EWK_TOUCH_START:
        type = WebCore::PlatformEvent::TouchStart;
        break;
    case EWK_TOUCH_MOVE:
        type = WebCore::PlatformEvent::TouchMove;
        break;
    case EWK_TOUCH_END:
        type = WebCore::PlatformEvent::TouchEnd;
        break;
    case EWK_TOUCH_CANCEL:
        type = WebCore::PlatformEvent::TouchCancel;
        break;
    default:
        ASSERT_NOT_REACHED();
        return false;
    }

    WebCore::PlatformTouchEvent touchEvent(points, WebCore::IntPoint(x, y), type, metaState);
    return smartData->frame->eventHandler()->handleTouchEvent(touchEvent);
#else
    return false;
#endif
}
