static void _ewk_view_smart_add(Evas_Object* ewkView)
{
    const Evas_Smart* smart = evas_object_smart_smart_get(ewkView);
    const Evas_Smart_Class* smartClass = evas_smart_class_get(smart);
    const Ewk_View_Smart_Class* api = reinterpret_cast<const Ewk_View_Smart_Class*>(smartClass);
    EINA_SAFETY_ON_NULL_RETURN(api->backing_store_add);
    EWK_VIEW_SD_GET(ewkView, smartData);

    if (!smartData) {
        smartData = static_cast<Ewk_View_Smart_Data*>(calloc(1, sizeof(Ewk_View_Smart_Data)));
        if (!smartData) {
            CRITICAL("could not allocate Ewk_View_Smart_Data");
            return;
        }
        evas_object_smart_data_set(ewkView, smartData);
    }

    smartData->bg_color.r = 255;
    smartData->bg_color.g = 255;
    smartData->bg_color.b = 255;
    smartData->bg_color.a = 255;

    smartData->self = ewkView;
    smartData->_priv = _ewk_view_priv_new(smartData);
    smartData->api = api;

    _parent_sc.add(ewkView);

    if (!smartData->_priv)
        return;

    EWK_VIEW_PRIV_GET(smartData, priv);

    smartData->backing_store = api->backing_store_add(smartData);
    if (!smartData->backing_store) {
        ERR("Could not create backing store object.");
        return;
    }

    evas_object_smart_member_add(smartData->backing_store, ewkView);
    evas_object_show(smartData->backing_store);
    evas_object_pass_events_set(smartData->backing_store, true);

    smartData->events_rect = evas_object_rectangle_add(smartData->base.evas);
    evas_object_color_set(smartData->events_rect, 0, 0, 0, 0);
    evas_object_smart_member_add(smartData->events_rect, ewkView);
    evas_object_show(smartData->events_rect);

    smartData->main_frame = ewk_frame_add(smartData->base.evas);
    if (!smartData->main_frame) {
        ERR("Could not create main frame object.");
        return;
    }

    if (!ewk_frame_init(smartData->main_frame, ewkView, priv->mainFrame)) {
        ERR("Could not initialize main frme object.");
        evas_object_del(smartData->main_frame);
        smartData->main_frame = 0;

        delete priv->mainFrame;
        priv->mainFrame = 0;
        return;
    }

    evas_object_name_set(smartData->main_frame, "EWK_Frame:main");
    evas_object_smart_member_add(smartData->main_frame, ewkView);
    evas_object_show(smartData->main_frame);

#define CONNECT(s, c) evas_object_event_callback_add(ewkView, s, c, smartData)
    CONNECT(EVAS_CALLBACK_FOCUS_IN, _ewk_view_on_focus_in);
    CONNECT(EVAS_CALLBACK_FOCUS_OUT, _ewk_view_on_focus_out);
    CONNECT(EVAS_CALLBACK_MOUSE_WHEEL, _ewk_view_on_mouse_wheel);
    CONNECT(EVAS_CALLBACK_MOUSE_DOWN, _ewk_view_on_mouse_down);
    CONNECT(EVAS_CALLBACK_MOUSE_UP, _ewk_view_on_mouse_up);
    CONNECT(EVAS_CALLBACK_MOUSE_MOVE, _ewk_view_on_mouse_move);
    CONNECT(EVAS_CALLBACK_KEY_DOWN, _ewk_view_on_key_down);
    CONNECT(EVAS_CALLBACK_KEY_UP, _ewk_view_on_key_up);
#undef CONNECT
}
