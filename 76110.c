static void _ewk_view_smart_del(Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET(ewkView, smartData);
    Ewk_View_Private_Data* priv = smartData ? smartData->_priv : 0;

    ewk_view_stop(ewkView);
    _parent_sc.del(ewkView);
    _ewk_view_priv_del(priv);
}
