Eina_Bool ewk_frame_back_possible(Evas_Object* ewkFrame)
{
    return ewk_frame_navigate_possible(ewkFrame, -1);
}
