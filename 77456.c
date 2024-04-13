Eina_Bool ewk_frame_forward_possible(Evas_Object* ewkFrame)
{
    return ewk_frame_navigate_possible(ewkFrame, 1);
}
