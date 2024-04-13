add_printer_state_reasons(
    cupsd_client_t  *con,		/* I - Client connection */
    cupsd_printer_t *p)			/* I - Printer info */
{
  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "add_printer_state_reasons(%p[%d], %p[%s])",
                  con, con->number, p, p->name);

  if (p->num_reasons == 0)
    ippAddString(con->response, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
                 "printer-state-reasons", NULL, "none");
  else
    ippAddStrings(con->response, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
                  "printer-state-reasons", p->num_reasons, NULL,
		  (const char * const *)p->reasons);
}
