static Eina_Bool _ewk_view_smart_zoom_set(Ewk_View_Smart_Data* smartData, float zoom, Evas_Coord centerX, Evas_Coord centerY)
{
    double px, py;
    Evas_Coord x, y, width, height;
    Eina_Bool result;

    ewk_frame_scroll_size_get(smartData->main_frame, &width, &height);
    ewk_frame_scroll_pos_get(smartData->main_frame, &x, &y);

    if (width + smartData->view.w > 0)
        px = static_cast<double>(x + centerX) / (width + smartData->view.w);
    else
        px = 0.0;

    if (height + smartData->view.h > 0)
        py = static_cast<double>(y + centerY) / (height + smartData->view.h);
    else
        py = 0.0;

    result = ewk_frame_page_zoom_set(smartData->main_frame, zoom);

    ewk_frame_scroll_size_get(smartData->main_frame, &width, &height);
    x = (width + smartData->view.w) * px - centerX;
    y = (height + smartData->view.h) * py - centerY;
    ewk_frame_scroll_set(smartData->main_frame, x, y);
    return result;
}
