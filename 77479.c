void ewk_frame_paint_full_set(Evas_Object* ewkFrame, Eina_Bool flag)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData);
    EINA_SAFETY_ON_NULL_RETURN(smartData->frame);
    EINA_SAFETY_ON_NULL_RETURN(smartData->frame->view());
    smartData->frame->view()->setPaintsEntireContents(flag);
}
