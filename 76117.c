static Eina_Bool _ewk_view_smart_key_down(Ewk_View_Smart_Data* smartData, const Evas_Event_Key_Down* downEvent)
{
    Evas_Object* frame = ewk_view_frame_focused_get(smartData->self);

    if (!frame)
        frame = smartData->main_frame;

    return ewk_frame_feed_key_down(frame, downEvent);
}
