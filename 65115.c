UpdateActionMods(struct xkb_keymap *keymap, union xkb_action *act,
                 xkb_mod_mask_t modmap)
{
    switch (act->type) {
    case ACTION_TYPE_MOD_SET:
    case ACTION_TYPE_MOD_LATCH:
    case ACTION_TYPE_MOD_LOCK:
        if (act->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            act->mods.mods.mods = modmap;
        ComputeEffectiveMask(keymap, &act->mods.mods);
        break;
    default:
        break;
    }
}
