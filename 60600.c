static void rtsp_reply_error(HTTPContext *c, enum RTSPStatusCode error_number)
{
    rtsp_reply_header(c, error_number);
    avio_printf(c->pb, "\r\n");
}
