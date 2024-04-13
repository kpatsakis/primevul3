static int print_statusline(const char *extname, unsigned statusitems,
                            struct statusdata *sd)
{
    int sepchar;

    prot_printf(imapd_out, "* STATUS ");
    prot_printastring(imapd_out, extname);
    prot_printf(imapd_out, " ");
    sepchar = '(';

    if (statusitems & STATUS_MESSAGES) {
        prot_printf(imapd_out, "%cMESSAGES %u", sepchar, sd->messages);
        sepchar = ' ';
    }
    if (statusitems & STATUS_RECENT) {
        prot_printf(imapd_out, "%cRECENT %u", sepchar, sd->recent);
        sepchar = ' ';
    }
    if (statusitems & STATUS_UIDNEXT) {
        prot_printf(imapd_out, "%cUIDNEXT %u", sepchar, sd->uidnext);
        sepchar = ' ';
    }
    if (statusitems & STATUS_UIDVALIDITY) {
        prot_printf(imapd_out, "%cUIDVALIDITY %u", sepchar, sd->uidvalidity);
        sepchar = ' ';
    }
    if (statusitems & STATUS_UNSEEN) {
        prot_printf(imapd_out, "%cUNSEEN %u", sepchar, sd->unseen);
        sepchar = ' ';
    }
    if (statusitems & STATUS_HIGHESTMODSEQ) {
        prot_printf(imapd_out, "%cHIGHESTMODSEQ " MODSEQ_FMT,
                    sepchar, sd->highestmodseq);
        sepchar = ' ';
    }
    if (statusitems & STATUS_XCONVEXISTS) {
        prot_printf(imapd_out, "%cXCONVEXISTS %u", sepchar, sd->xconv.exists);
        sepchar = ' ';
    }
    if (statusitems & STATUS_XCONVUNSEEN) {
        prot_printf(imapd_out, "%cXCONVUNSEEN %u", sepchar, sd->xconv.unseen);
        sepchar = ' ';
    }
    if (statusitems & STATUS_XCONVMODSEQ) {
        prot_printf(imapd_out, "%cXCONVMODSEQ " MODSEQ_FMT, sepchar, sd->xconv.modseq);
        sepchar = ' ';
    }

    prot_printf(imapd_out, ")\r\n");

    return 0;
}
