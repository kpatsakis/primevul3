Eina_Bool ewk_view_setting_local_storage_database_path_set(Evas_Object* ewkView, const char* path)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    if (eina_stringshare_replace(&priv->settings.localStorageDatabasePath, path))
        priv->pageSettings->setLocalStorageDatabasePath(String::fromUTF8(path));
    return true;
}
