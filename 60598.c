static void rtsp_cmd_play(HTTPContext *c, const char *url, RTSPMessageHeader *h)
{
    HTTPContext *rtp_c;

    rtp_c = find_rtp_session_with_url(url, h->session_id);
    if (!rtp_c) {
        rtsp_reply_error(c, RTSP_STATUS_SESSION);
        return;
    }

    if (rtp_c->state != HTTPSTATE_SEND_DATA &&
        rtp_c->state != HTTPSTATE_WAIT_FEED &&
        rtp_c->state != HTTPSTATE_READY) {
        rtsp_reply_error(c, RTSP_STATUS_STATE);
        return;
    }

    rtp_c->state = HTTPSTATE_SEND_DATA;

    /* now everything is OK, so we can send the connection parameters */
    rtsp_reply_header(c, RTSP_STATUS_OK);
    /* session ID */
    avio_printf(c->pb, "Session: %s\r\n", rtp_c->session_id);
    avio_printf(c->pb, "\r\n");
}
