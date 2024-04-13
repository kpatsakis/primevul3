accept_jobs(cupsd_client_t  *con,	/* I - Client connection */
            ipp_attribute_t *uri)	/* I - Printer or class URI */
{
  http_status_t	status;			/* Policy status */
  cups_ptype_t	dtype;			/* Destination type (printer/class) */
  cupsd_printer_t *printer;		/* Printer data */


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "accept_jobs(%p[%d], %s)", con,
                  con->number, uri->values[0].string.text);

 /*
  * Is the destination valid?
  */

  if (!cupsdValidateDest(uri->values[0].string.text, &dtype, &printer))
  {
   /*
    * Bad URI...
    */

    send_ipp_status(con, IPP_NOT_FOUND,
                    _("The printer or class does not exist."));
    return;
  }

 /*
  * Check policy...
  */

  if ((status = cupsdCheckPolicy(printer->op_policy_ptr, con, NULL)) != HTTP_OK)
  {
    send_http_error(con, status, printer);
    return;
  }

 /*
  * Accept jobs sent to the printer...
  */

  printer->accepting        = 1;
  printer->state_message[0] = '\0';

  cupsdAddEvent(CUPSD_EVENT_PRINTER_STATE, printer, NULL,
                "Now accepting jobs.");

  if (dtype & CUPS_PRINTER_CLASS)
  {
    cupsdMarkDirty(CUPSD_DIRTY_CLASSES);

    cupsdLogMessage(CUPSD_LOG_INFO, "Class \"%s\" now accepting jobs (\"%s\").",
                    printer->name, get_username(con));
  }
  else
  {
    cupsdMarkDirty(CUPSD_DIRTY_PRINTERS);

    cupsdLogMessage(CUPSD_LOG_INFO,
                    "Printer \"%s\" now accepting jobs (\"%s\").",
                    printer->name, get_username(con));
  }

 /*
  * Everything was ok, so return OK status...
  */

  con->response->request.status.status_code = IPP_OK;
}
