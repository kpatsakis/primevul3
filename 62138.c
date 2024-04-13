hb_shaper_get_run_language(ASS_Shaper *shaper, hb_script_t script)
{
    hb_language_t lang;

    if (shaper->language != HB_LANGUAGE_INVALID)
        return shaper->language;

    lang = script_to_language(script);

    if (lang == HB_LANGUAGE_INVALID)
        lang = hb_language_get_default();

    return lang;
}
