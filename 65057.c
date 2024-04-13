CopyKeyNamesInfoToKeymap(struct xkb_keymap *keymap, KeyNamesInfo *info)
{
    /* This function trashes keymap on error, but that's OK. */
    if (!CopyKeyNamesToKeymap(keymap, info) ||
        !CopyKeyAliasesToKeymap(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        return false;

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    return true;
}
