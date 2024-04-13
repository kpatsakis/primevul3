static int asf_read_generic_value(AVIOContext *pb, int type, uint64_t *value)
{

    switch (type) {
    case ASF_BOOL:
        *value = avio_rl16(pb);
        break;
    case ASF_DWORD:
        *value = avio_rl32(pb);
        break;
    case ASF_QWORD:
        *value = avio_rl64(pb);
        break;
    case ASF_WORD:
        *value = avio_rl16(pb);
        break;
    default:
        return AVERROR_INVALIDDATA;
    }

    return 0;
}
