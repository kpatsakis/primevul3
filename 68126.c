gplotSetScaling(GPLOT   *gplot,
                l_int32  scaling)
{
    PROCNAME("gplotSetScaling");

    if (!gplot)
        return ERROR_INT("gplot not defined", procName, 1);
    if (scaling != GPLOT_LINEAR_SCALE &&
        scaling != GPLOT_LOG_SCALE_X &&
        scaling != GPLOT_LOG_SCALE_Y &&
        scaling != GPLOT_LOG_SCALE_X_Y)
        return ERROR_INT("invalid gplot scaling", procName, 1);
    gplot->scaling = scaling;
    return 0;
}
