static void _ewk_view_priv_del(Ewk_View_Private_Data* priv)
{
    if (!priv)
        return;

    /* do not delete priv->main_frame */

    free(priv->repaints.array);
    free(priv->scrolls.array);

    eina_stringshare_del(priv->settings.userAgent);
    eina_stringshare_del(priv->settings.userStylesheet);
    eina_stringshare_del(priv->settings.encodingDefault);
    eina_stringshare_del(priv->settings.encodingCustom);
    eina_stringshare_del(priv->settings.fontStandard);
    eina_stringshare_del(priv->settings.fontCursive);
    eina_stringshare_del(priv->settings.fontMonospace);
    eina_stringshare_del(priv->settings.fontFantasy);
    eina_stringshare_del(priv->settings.fontSerif);
    eina_stringshare_del(priv->settings.fontSansSerif);
    eina_stringshare_del(priv->settings.localStorageDatabasePath);

    if (priv->animatedZoom.animator)
        ecore_animator_del(priv->animatedZoom.animator);

    ewk_history_free(priv->history);

    delete priv->page;
    free(priv);
}
