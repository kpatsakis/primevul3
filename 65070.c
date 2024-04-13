AppendStmt(ParseCommon *to, ParseCommon *append)
{
    ParseCommon *iter;

    if (!to)
        return append;

    for (iter = to; iter->next; iter = iter->next);

    iter->next = append;
    return to;
}
