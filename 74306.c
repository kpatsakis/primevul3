AppLayerProtoDetectProbingParserElementDuplicate(AppLayerProtoDetectProbingParserElement *pe)
{
    SCEnter();

    AppLayerProtoDetectProbingParserElement *new_pe = AppLayerProtoDetectProbingParserElementAlloc();

    new_pe->alproto = pe->alproto;
    new_pe->port = pe->port;
    new_pe->alproto_mask = pe->alproto_mask;
    new_pe->min_depth = pe->min_depth;
    new_pe->max_depth = pe->max_depth;
    new_pe->ProbingParserTs = pe->ProbingParserTs;
    new_pe->ProbingParserTc = pe->ProbingParserTc;
    new_pe->next = NULL;

    SCReturnPtr(new_pe, "AppLayerProtoDetectProbingParserElement");
}
