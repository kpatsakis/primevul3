static Eina_Bool _ewk_view_scrolls_resize(Ewk_View_Private_Data* priv, size_t size)
{
    void* tmp = realloc(priv->scrolls.array, size * sizeof(Ewk_Scroll_Request));
    if (!tmp) {
        CRITICAL("could not realloc scrolls array to %zu elements.", size);
        return false;
    }
    priv->scrolls.allocated = size;
    priv->scrolls.array = static_cast<Ewk_Scroll_Request*>(tmp);
    return true;
}
