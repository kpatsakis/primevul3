static Eina_Bool _ewk_view_smart_mouse_down(Ewk_View_Smart_Data* smartData, const Evas_Event_Mouse_Down* downEvent)
{
    return ewk_frame_feed_mouse_down(smartData->main_frame, downEvent);
}
