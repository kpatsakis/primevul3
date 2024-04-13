void ewk_frame_core_gone(Evas_Object* ewkFrame)
{
    DBG("ewkFrame=%p", ewkFrame);
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData);
    smartData->frame = 0;
}
