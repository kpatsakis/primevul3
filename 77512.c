const char* ewk_frame_uri_get(const Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);
    return smartData->uri;
}
