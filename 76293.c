Eina_Bool ewk_view_visibility_state_set(Evas_Object* ewkView, Ewk_Page_Visibility_State pageVisibilityState, Eina_Bool initialState)
{
#if ENABLE(PAGE_VISIBILITY_API)
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);

    priv->page->setVisibilityState(static_cast<WebCore::PageVisibilityState>(pageVisibilityState), initialState);

    return true;
#else
    DBG("PAGE_VISIBILITY_API is disabled.");
    return false;
#endif
}
