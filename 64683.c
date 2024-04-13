parse_instruction_ids(struct ofpbuf *payload, bool loose, uint32_t *insts)
{
    *insts = 0;
    while (payload->size > 0) {
        enum ovs_instruction_type inst;
        enum ofperr error;
        uint64_t ofpit;

        /* OF1.3 and OF1.4 aren't clear about padding in the instruction IDs.
         * It seems clear that they aren't padded to 8 bytes, though, because
         * both standards say that "non-experimenter instructions are 4 bytes"
         * and do not mention any padding before the first instruction ID.
         * (There wouldn't be any point in padding to 8 bytes if the IDs were
         * aligned on an odd 4-byte boundary.)
         *
         * Anyway, we just assume they're all glommed together on byte
         * boundaries. */
        error = ofpprop_pull__(payload, NULL, 1, 0x10000, &ofpit);
        if (error) {
            return error;
        }

        error = ovs_instruction_type_from_inst_type(&inst, ofpit);
        if (!error) {
            *insts |= 1u << inst;
        } else if (!loose) {
            return error;
        }
    }
    return 0;
}
