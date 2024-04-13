ReportSINotArray(CompatInfo *info, SymInterpInfo *si, const char *field)
{
    return ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}
