DecodeThreadVars *DecodeThreadVarsAlloc(ThreadVars *tv)
{
    DecodeThreadVars *dtv = NULL;

    if ( (dtv = SCMalloc(sizeof(DecodeThreadVars))) == NULL)
        return NULL;
    memset(dtv, 0, sizeof(DecodeThreadVars));

    dtv->app_tctx = AppLayerGetCtxThread(tv);

    if (OutputFlowLogThreadInit(tv, NULL, &dtv->output_flow_thread_data) != TM_ECODE_OK) {
        SCLogError(SC_ERR_THREAD_INIT, "initializing flow log API for thread failed");
        DecodeThreadVarsFree(tv, dtv);
        return NULL;
    }

    /** set config defaults */
    int vlanbool = 0;
    if ((ConfGetBool("vlan.use-for-tracking", &vlanbool)) == 1 && vlanbool == 0) {
        dtv->vlan_disabled = 1;
    }
    SCLogDebug("vlan tracking is %s", dtv->vlan_disabled == 0 ? "enabled" : "disabled");

    return dtv;
}
