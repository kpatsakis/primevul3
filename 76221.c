const Ewk_Scroll_Request* ewk_view_scroll_requests_get(const Ewk_View_Private_Data* priv, size_t* count)
{
    if (count)
        *count = 0;
    EINA_SAFETY_ON_NULL_RETURN_VAL(priv, 0);
    if (count)
        *count = priv->scrolls.count;
    return priv->scrolls.array;
}
