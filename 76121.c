static Eina_Bool _ewk_view_smart_mouse_wheel(Ewk_View_Smart_Data* smartData, const Evas_Event_Mouse_Wheel* wheelEvent)
{
    return ewk_frame_feed_mouse_wheel(smartData->main_frame, wheelEvent);
}
