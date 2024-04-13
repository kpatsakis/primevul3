InitAliasInfo(AliasInfo *info, enum merge_mode merge,
              xkb_atom_t alias, xkb_atom_t real)
{
    memset(info, 0, sizeof(*info));
    info->merge = merge;
    info->alias = alias;
    info->real = real;
}
