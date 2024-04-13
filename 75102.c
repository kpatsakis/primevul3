static gboolean timeout_cb(gpointer)
{
    if (sharedTimerFiredFunction)
        sharedTimerFiredFunction();
    return FALSE;
}
