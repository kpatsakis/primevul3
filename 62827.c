static int asf_read_packet_header(AVFormatContext *s)
{
    ASFContext *asf = s->priv_data;
    AVIOContext *pb = s->pb;
    uint64_t size;
    uint32_t av_unused seq;
    unsigned char error_flags, len_flags, pay_flags;

    asf->packet_offset = avio_tell(pb);
    error_flags = avio_r8(pb); // read Error Correction Flags
    if (error_flags & ASF_PACKET_FLAG_ERROR_CORRECTION_PRESENT) {
        if (!(error_flags & ASF_ERROR_CORRECTION_LENGTH_TYPE)) {
            size = error_flags & ASF_PACKET_ERROR_CORRECTION_DATA_SIZE;
            avio_skip(pb, size);
        }
        len_flags       = avio_r8(pb);
    } else
        len_flags = error_flags;
    asf->prop_flags = avio_r8(pb);
    READ_LEN(len_flags & ASF_PPI_MASK_PACKET_LENGTH_FIELD_SIZE,
             ASF_PPI_FLAG_PACKET_LENGTH_FIELD_, asf->packet_size_internal);
    READ_LEN(len_flags & ASF_PPI_MASK_SEQUENCE_FIELD_SIZE,
             ASF_PPI_FLAG_SEQUENCE_FIELD_, seq);
    READ_LEN(len_flags & ASF_PPI_MASK_PADDING_LENGTH_FIELD_SIZE,
             ASF_PPI_FLAG_PADDING_LENGTH_FIELD_, asf->pad_len );
    asf->send_time = avio_rl32(pb); // send time
    avio_skip(pb, 2); // skip duration
    if (len_flags & ASF_PPI_FLAG_MULTIPLE_PAYLOADS_PRESENT) { // Multiple Payloads present
        pay_flags = avio_r8(pb);
        asf->nb_mult_left = (pay_flags & ASF_NUM_OF_PAYLOADS);
    }

    return 0;
}
