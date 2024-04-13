void InlineFlowBox::clearTruncation()
{
    for (InlineBox *box = firstChild(); box; box = box->nextOnLine())
        box->clearTruncation();
}
