ComputeEffectiveMask(struct xkb_keymap *keymap, struct xkb_mods *mods)
{
    mods->mask = mod_mask_get_effective(keymap, mods->mods);
}
