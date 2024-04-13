Eina_Bool ewk_view_base_smart_set(Ewk_View_Smart_Class* api)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(api, false);

    if (api->version != EWK_VIEW_SMART_CLASS_VERSION) {
        EINA_LOG_CRIT
            ("Ewk_View_Smart_Class %p is version %lu while %lu was expected.",
            api, api->version, EWK_VIEW_SMART_CLASS_VERSION);
        return false;
    }

    if (EINA_UNLIKELY(!_parent_sc.add))
        evas_object_smart_clipped_smart_set(&_parent_sc);

    evas_object_smart_clipped_smart_set(&api->sc);
    api->sc.add = _ewk_view_smart_add;
    api->sc.del = _ewk_view_smart_del;
    api->sc.resize = _ewk_view_smart_resize;
    api->sc.move = _ewk_view_smart_move;
    api->sc.calculate = _ewk_view_smart_calculate;
    api->sc.show = _ewk_view_smart_show;
    api->sc.hide = _ewk_view_smart_hide;
    api->sc.data = EWK_VIEW_TYPE_STR; /* used by type checking */
    api->sc.callbacks = _ewk_view_callback_names;

    api->contents_resize = _ewk_view_smart_contents_resize;
    api->zoom_set = _ewk_view_smart_zoom_set;
    api->flush = _ewk_view_smart_flush;
    api->pre_render_region = _ewk_view_smart_pre_render_region;
    api->pre_render_relative_radius = _ewk_view_smart_pre_render_relative_radius;
    api->pre_render_cancel = _ewk_view_smart_pre_render_cancel;
    api->disable_render = _ewk_view_smart_disable_render;
    api->enable_render = _ewk_view_smart_enable_render;

    api->focus_in = _ewk_view_smart_focus_in;
    api->focus_out = _ewk_view_smart_focus_out;
    api->mouse_wheel = _ewk_view_smart_mouse_wheel;
    api->mouse_down = _ewk_view_smart_mouse_down;
    api->mouse_up = _ewk_view_smart_mouse_up;
    api->mouse_move = _ewk_view_smart_mouse_move;
    api->key_down = _ewk_view_smart_key_down;
    api->key_up = _ewk_view_smart_key_up;

    api->add_console_message = _ewk_view_smart_add_console_message;
    api->run_javascript_alert = _ewk_view_smart_run_javascript_alert;
    api->run_javascript_confirm = _ewk_view_smart_run_javascript_confirm;
    api->run_javascript_prompt = _ewk_view_smart_run_javascript_prompt;
    api->should_interrupt_javascript = _ewk_view_smart_should_interrupt_javascript;

    return true;
}
