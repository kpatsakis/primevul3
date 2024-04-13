DetectPrefilterSetNonPrefilterList(const Packet *p, DetectEngineThreadCtx *det_ctx, DetectRunScratchpad *scratch)
{
    if ((p->proto == IPPROTO_TCP) && (p->tcph != NULL) && (p->tcph->th_flags & TH_SYN)) {
        det_ctx->non_pf_store_ptr = scratch->sgh->non_pf_syn_store_array;
        det_ctx->non_pf_store_cnt = scratch->sgh->non_pf_syn_store_cnt;
    } else {
        det_ctx->non_pf_store_ptr = scratch->sgh->non_pf_other_store_array;
        det_ctx->non_pf_store_cnt = scratch->sgh->non_pf_other_store_cnt;
    }
    SCLogDebug("sgh non_pf ptr %p cnt %u (syn %p/%u, other %p/%u)",
            det_ctx->non_pf_store_ptr, det_ctx->non_pf_store_cnt,
            scratch->sgh->non_pf_syn_store_array, scratch->sgh->non_pf_syn_store_cnt,
            scratch->sgh->non_pf_other_store_array, scratch->sgh->non_pf_other_store_cnt);
}
