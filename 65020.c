CompileCompatMap(XkbFile *file, struct xkb_keymap *keymap,
                 enum merge_mode merge)
{
    CompatInfo info;
    ActionsInfo *actions;

    actions = NewActionsInfo();
    if (!actions)
        return false;

    InitCompatInfo(&info, keymap->ctx, actions, &keymap->mods);
    info.default_interp.merge = merge;
    info.default_led.merge = merge;

    HandleCompatMapFile(&info, file, merge);
    if (info.errorCount != 0)
        goto err_info;

    if (!CopyCompatToKeymap(keymap, &info))
        goto err_info;

    ClearCompatInfo(&info);
    FreeActionsInfo(actions);
    return true;

err_info:
    ClearCompatInfo(&info);
    FreeActionsInfo(actions);
    return false;
}
