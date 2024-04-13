FreeInclude(IncludeStmt *incl)
{
    IncludeStmt *next;

    while (incl)
    {
        next = incl->next_incl;

        free(incl->file);
        free(incl->map);
        free(incl->modifier);
        free(incl->stmt);

        free(incl);
        incl = next;
    }
}
