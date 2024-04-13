Eina_Bool ewk_frame_feed_mouse_wheel(Evas_Object* ewkFrame, const Evas_Event_Mouse_Wheel* wheelEvent)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(wheelEvent, false);

    WebCore::FrameView* view = smartData->frame->view();
    DBG("ewkFrame=%p, view=%p, direction=%d, z=%d, pos=%d,%d",
        ewkFrame, view, wheelEvent->direction, wheelEvent->z, wheelEvent->canvas.x, wheelEvent->canvas.y);
    EINA_SAFETY_ON_NULL_RETURN_VAL(view, false);

    WebCore::PlatformWheelEvent event(wheelEvent);
    return smartData->frame->eventHandler()->handleWheelEvent(event);
}
