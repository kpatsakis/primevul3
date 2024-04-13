static Eina_Bool _ewk_view_repaints_resize(Ewk_View_Private_Data* priv, size_t size)
{
    void* tmp = realloc(priv->repaints.array, size * sizeof(Eina_Rectangle));
    if (!tmp) {
        CRITICAL("could not realloc repaints array to %zu elements.", size);
        return false;
    }
    priv->repaints.allocated = size;
    priv->repaints.array = static_cast<Eina_Rectangle*>(tmp);
    return true;
}
