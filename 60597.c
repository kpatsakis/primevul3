static void rtsp_cmd_options(HTTPContext *c, const char *url)
{
    /* rtsp_reply_header(c, RTSP_STATUS_OK); */
    avio_printf(c->pb, "RTSP/1.0 %d %s\r\n", RTSP_STATUS_OK, "OK");
    avio_printf(c->pb, "CSeq: %d\r\n", c->seq);
    avio_printf(c->pb, "Public: %s\r\n",
                "OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE");
    avio_printf(c->pb, "\r\n");
}
