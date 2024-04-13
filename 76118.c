static Eina_Bool _ewk_view_smart_key_up(Ewk_View_Smart_Data* smartData, const Evas_Event_Key_Up* upEvent)
{
    Evas_Object* frame = ewk_view_frame_focused_get(smartData->self);

    if (!frame)
        frame = smartData->main_frame;

    return ewk_frame_feed_key_up(frame, upEvent);
}
