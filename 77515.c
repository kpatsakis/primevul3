Evas_Object* ewk_frame_view_get(const Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);
    return smartData->view;
}
