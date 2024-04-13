ReportLedNotArray(CompatInfo *info, LedInfo *ledi, const char *field)
{
    return ReportNotArray(info->ctx, "indicator map", field,
                          xkb_atom_text(info->ctx, ledi->led.name));
}
