static Eina_Bool _ewk_view_smart_mouse_move(Ewk_View_Smart_Data* smartData, const Evas_Event_Mouse_Move* moveEvent)
{
    return ewk_frame_feed_mouse_move(smartData->main_frame, moveEvent);
}
