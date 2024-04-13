DetectPrefilterBuildNonPrefilterList(DetectEngineThreadCtx *det_ctx, SignatureMask mask, uint8_t alproto)
{
    uint32_t x = 0;
    for (x = 0; x < det_ctx->non_pf_store_cnt; x++) {
        /* only if the mask matches this rule can possibly match,
         * so build the non_mpm array only for match candidates */
        const SignatureMask rule_mask = det_ctx->non_pf_store_ptr[x].mask;
        const uint8_t rule_alproto = det_ctx->non_pf_store_ptr[x].alproto;
        if ((rule_mask & mask) == rule_mask && (rule_alproto == 0 || rule_alproto == alproto)) {
            det_ctx->non_pf_id_array[det_ctx->non_pf_id_cnt++] = det_ctx->non_pf_store_ptr[x].id;
        }
    }
}
