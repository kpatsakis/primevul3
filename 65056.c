CompileKeycodes(XkbFile *file, struct xkb_keymap *keymap,
                enum merge_mode merge)
{
    KeyNamesInfo info;

    InitKeyNamesInfo(&info, keymap->ctx);

    HandleKeycodesFile(&info, file, merge);
    if (info.errorCount != 0)
        goto err_info;

    if (!CopyKeyNamesInfoToKeymap(keymap, &info))
        goto err_info;

    ClearKeyNamesInfo(&info);
    return true;

err_info:
    ClearKeyNamesInfo(&info);
    return false;
}
