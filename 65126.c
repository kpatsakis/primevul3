static void put_amf_dword_array(AVIOContext *pb, uint32_t dw)
{
    avio_w8(pb, AMF_DATA_TYPE_ARRAY);
    avio_wb32(pb, dw);
}
