static void ipa_device_close(wmfAPI * API)
{
  wmf_magick_t
    *ddata = WMF_MAGICK_GetData(API);

  DestroyDrawingWand(ddata->draw_wand);
  DestroyDrawInfo(ddata->draw_info);
  RelinquishMagickMemory(WMF_MAGICK_GetFontData(API)->ps_name);
}
