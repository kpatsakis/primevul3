AppLayerProtoDetectProbingParserElementCreate(AppProto alproto,
                                              uint16_t port,
                                              uint16_t min_depth,
                                              uint16_t max_depth)
{
    AppLayerProtoDetectProbingParserElement *pe = AppLayerProtoDetectProbingParserElementAlloc();

    pe->alproto = alproto;
    pe->port = port;
    pe->alproto_mask = AppLayerProtoDetectProbingParserGetMask(alproto);
    pe->min_depth = min_depth;
    pe->max_depth = max_depth;
    pe->next = NULL;

    if (max_depth != 0 && min_depth >= max_depth) {
        SCLogError(SC_ERR_ALPARSER, "Invalid arguments sent to "
                   "register the probing parser.  min_depth >= max_depth");
        goto error;
    }
    if (alproto <= ALPROTO_UNKNOWN || alproto >= ALPROTO_MAX) {
        SCLogError(SC_ERR_ALPARSER, "Invalid arguments sent to register "
                   "the probing parser.  Invalid alproto - %d", alproto);
        goto error;
    }

    SCReturnPtr(pe, "AppLayerProtoDetectProbingParserElement");
 error:
    AppLayerProtoDetectProbingParserElementFree(pe);
    SCReturnPtr(NULL, "AppLayerProtoDetectProbingParserElement");
}
