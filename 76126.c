static Eina_Bool _ewk_view_smart_run_javascript_confirm(Ewk_View_Smart_Data* smartData, Evas_Object* frame, const char* message)
{
    INF("javascript confirm: %s", message);
    INF("javascript confirm (HARD CODED)? YES");
    return true;
}
