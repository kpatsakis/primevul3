void DecodeThreadVarsFree(ThreadVars *tv, DecodeThreadVars *dtv)
{
    if (dtv != NULL) {
        if (dtv->app_tctx != NULL)
            AppLayerDestroyCtxThread(dtv->app_tctx);

        if (dtv->output_flow_thread_data != NULL)
            OutputFlowLogThreadDeinit(tv, dtv->output_flow_thread_data);

        SCFree(dtv);
    }
}
