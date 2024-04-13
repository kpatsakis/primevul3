Ewk_Page_Visibility_State ewk_view_visibility_state_get(const Evas_Object* ewkView)
{
#if ENABLE(PAGE_VISIBILITY_API)
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, EWK_PAGE_VISIBILITY_STATE_VISIBLE);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, EWK_PAGE_VISIBILITY_STATE_VISIBLE);

    return static_cast<Ewk_Page_Visibility_State>(priv->page->visibilityState());
#else
    DBG("PAGE_VISIBILITY_API is disabled.");
    return EWK_PAGE_VISIBILITY_STATE_VISIBLE;
#endif
}
