static void _ewk_view_zoom_animated_mark_stop(Ewk_View_Smart_Data* smartData)
{
    smartData->animated_zoom.zoom.start = 0.0;
    smartData->animated_zoom.zoom.end = 0.0;
    smartData->animated_zoom.zoom.current = 0.0;
}
