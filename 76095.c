static void _ewk_view_on_mouse_up(void* data, Evas* eventType, Evas_Object* callback, void* eventInfo)
{
    Evas_Event_Mouse_Up* upEvent = static_cast<Evas_Event_Mouse_Up*>(eventInfo);
    Ewk_View_Smart_Data* smartData = static_cast<Ewk_View_Smart_Data*>(data);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api->mouse_up);
    smartData->api->mouse_up(smartData, upEvent);
}
