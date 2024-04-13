decode_NXAST_RAW_LEARN(const struct nx_action_learn *nal,
                       enum ofp_version ofp_version OVS_UNUSED,
                       const struct vl_mff_map *vl_mff_map,
                       uint64_t *tlv_bitmap, struct ofpbuf *ofpacts)
{
    struct ofpact_learn *learn;
    const void *p, *end;

    if (nal->pad) {
        return OFPERR_OFPBAC_BAD_ARGUMENT;
    }

    learn = ofpact_put_LEARN(ofpacts);

    learn->idle_timeout = ntohs(nal->idle_timeout);
    learn->hard_timeout = ntohs(nal->hard_timeout);
    learn->priority = ntohs(nal->priority);
    learn->cookie = nal->cookie;
    learn->table_id = nal->table_id;
    learn->fin_idle_timeout = ntohs(nal->fin_idle_timeout);
    learn->fin_hard_timeout = ntohs(nal->fin_hard_timeout);

    learn->flags = ntohs(nal->flags);
    if (learn->flags & ~(NX_LEARN_F_SEND_FLOW_REM |
                         NX_LEARN_F_DELETE_LEARNED)) {
        return OFPERR_OFPBAC_BAD_ARGUMENT;
    }

    if (learn->table_id == 0xff) {
        return OFPERR_OFPBAC_BAD_ARGUMENT;
    }

    end = (char *) nal + ntohs(nal->len);
    for (p = nal + 1; p != end; ) {
        struct ofpact_learn_spec *spec;
        uint16_t header = ntohs(get_be16(&p));

        if (!header) {
            break;
        }

        spec = ofpbuf_put_zeros(ofpacts, sizeof *spec);
        learn = ofpacts->header;

        spec->src_type = header & NX_LEARN_SRC_MASK;
        spec->dst_type = header & NX_LEARN_DST_MASK;
        spec->n_bits = header & NX_LEARN_N_BITS_MASK;

        /* Check for valid src and dst type combination. */
        if (spec->dst_type == NX_LEARN_DST_MATCH ||
            spec->dst_type == NX_LEARN_DST_LOAD ||
            (spec->dst_type == NX_LEARN_DST_OUTPUT &&
             spec->src_type == NX_LEARN_SRC_FIELD)) {
            /* OK. */
        } else {
            return OFPERR_OFPBAC_BAD_ARGUMENT;
        }

        /* Check that the arguments don't overrun the end of the action. */
        if ((char *) end - (char *) p < learn_min_len(header)) {
            return OFPERR_OFPBAC_BAD_LEN;
        }

        /* Get the source. */
        const uint8_t *imm = NULL;
        unsigned int imm_bytes = 0;
        enum ofperr error;
        if (spec->src_type == NX_LEARN_SRC_FIELD) {
            error = get_subfield(spec->n_bits, &p, &spec->src, vl_mff_map,
                                 tlv_bitmap);
            if (error) {
                return error;
            }
        } else {
            int p_bytes = 2 * DIV_ROUND_UP(spec->n_bits, 16);
            p = (const uint8_t *) p + p_bytes;

            imm_bytes = DIV_ROUND_UP(spec->n_bits, 8);
            imm = (const uint8_t *) p - imm_bytes;
        }

        /* Get the destination. */
        if (spec->dst_type == NX_LEARN_DST_MATCH ||
            spec->dst_type == NX_LEARN_DST_LOAD) {
            error = get_subfield(spec->n_bits, &p, &spec->dst, vl_mff_map,
                                 tlv_bitmap);
            if (error) {
                return error;
            }
        }

        if (imm) {
            uint8_t *src_imm = ofpbuf_put_zeros(ofpacts,
                                                OFPACT_ALIGN(imm_bytes));
            memcpy(src_imm, imm, imm_bytes);

            learn = ofpacts->header;
        }
    }
    ofpact_finish_LEARN(ofpacts, &learn);

    if (!is_all_zeros(p, (char *) end - (char *) p)) {
        return OFPERR_OFPBAC_BAD_ARGUMENT;
    }

    return 0;
}
