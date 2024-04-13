InitCompatInfo(CompatInfo *info, struct xkb_context *ctx,
               ActionsInfo *actions, const struct xkb_mod_set *mods)
{
    memset(info, 0, sizeof(*info));
    info->ctx = ctx;
    info->actions = actions;
    info->mods = *mods;
    info->default_interp.merge = MERGE_OVERRIDE;
    info->default_interp.interp.virtual_mod = XKB_MOD_INVALID;
    info->default_led.merge = MERGE_OVERRIDE;
}
