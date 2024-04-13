WebCore::FloatRect ewk_view_page_rect_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET(ewkView, smartData);
    EWK_VIEW_PRIV_GET(smartData, priv);

    WebCore::Frame* main_frame = priv->page->mainFrame();
    return main_frame->view()->frameRect();
}
