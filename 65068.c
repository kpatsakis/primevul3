InitKeyNamesInfo(KeyNamesInfo *info, struct xkb_context *ctx)
{
    memset(info, 0, sizeof(*info));
    info->ctx = ctx;
    info->min_key_code = XKB_KEYCODE_INVALID;
#if XKB_KEYCODE_INVALID < XKB_KEYCODE_MAX
#error "Hey, you can't be changing stuff like that."
#endif
}
