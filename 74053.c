static void imapd_refer(const char *tag,
                        const char *server,
                        const char *mailbox)
{
    struct imapurl imapurl;
    char url[MAX_MAILBOX_PATH+1];

    memset(&imapurl, 0, sizeof(struct imapurl));
    imapurl.server = server;
    imapurl.mailbox = mailbox;
    imapurl.auth = !strcmp(imapd_userid, "anonymous") ? "anonymous" : "*";

    imapurl_toURL(url, &imapurl);

    prot_printf(imapd_out, "%s NO [REFERRAL %s] Remote mailbox.\r\n",
                tag, url);

    free(imapurl.freeme);
}
