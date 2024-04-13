void DecodeTeredoConfig(void)
{
    int enabled = 0;
    if (ConfGetBool("decoder.teredo.enabled", &enabled) == 1) {
        if (enabled) {
            g_teredo_enabled = true;
        } else {
            g_teredo_enabled = false;
        }
    }
}
