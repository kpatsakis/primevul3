add_class(cupsd_client_t  *con,		/* I - Client connection */
          ipp_attribute_t *uri)		/* I - URI of class */
{
  http_status_t	status;			/* Policy status */
  int		i;			/* Looping var */
  char		scheme[HTTP_MAX_URI],	/* Method portion of URI */
		username[HTTP_MAX_URI],	/* Username portion of URI */
		host[HTTP_MAX_URI],	/* Host portion of URI */
		resource[HTTP_MAX_URI];	/* Resource portion of URI */
  int		port;			/* Port portion of URI */
  cupsd_printer_t *pclass,		/* Class */
		*member;		/* Member printer/class */
  cups_ptype_t	dtype;			/* Destination type */
  ipp_attribute_t *attr;		/* Printer attribute */
  int		modify;			/* Non-zero if we just modified */
  int		need_restart_job;	/* Need to restart job? */


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "add_class(%p[%d], %s)", con,
                  con->number, uri->values[0].string.text);

 /*
  * Do we have a valid URI?
  */

  httpSeparateURI(HTTP_URI_CODING_ALL, uri->values[0].string.text, scheme,
                  sizeof(scheme), username, sizeof(username), host,
		  sizeof(host), &port, resource, sizeof(resource));


  if (strncmp(resource, "/classes/", 9) || strlen(resource) == 9)
  {
   /*
    * No, return an error...
    */

    send_ipp_status(con, IPP_BAD_REQUEST,
                    _("The printer-uri must be of the form "
		      "\"ipp://HOSTNAME/classes/CLASSNAME\"."));
    return;
  }

 /*
  * Do we have a valid printer name?
  */

  if (!validate_name(resource + 9))
  {
   /*
    * No, return an error...
    */

    send_ipp_status(con, IPP_BAD_REQUEST,
                    _("The printer-uri \"%s\" contains invalid characters."),
		    uri->values[0].string.text);
    return;
  }

 /*
  * See if the class already exists; if not, create a new class...
  */

  if ((pclass = cupsdFindClass(resource + 9)) == NULL)
  {
   /*
    * Class doesn't exist; see if we have a printer of the same name...
    */

    if ((pclass = cupsdFindPrinter(resource + 9)) != NULL)
    {
     /*
      * Yes, return an error...
      */

      send_ipp_status(con, IPP_NOT_POSSIBLE,
                      _("A printer named \"%s\" already exists."),
		      resource + 9);
      return;
    }

   /*
    * No, check the default policy and then add the class...
    */

    if ((status = cupsdCheckPolicy(DefaultPolicyPtr, con, NULL)) != HTTP_OK)
    {
      send_http_error(con, status, NULL);
      return;
    }

    pclass = cupsdAddClass(resource + 9);
    modify = 0;
  }
  else if ((status = cupsdCheckPolicy(pclass->op_policy_ptr, con,
                                      NULL)) != HTTP_OK)
  {
    send_http_error(con, status, pclass);
    return;
  }
  else
    modify = 1;

 /*
  * Look for attributes and copy them over as needed...
  */

  need_restart_job = 0;

  if ((attr = ippFindAttribute(con->request, "printer-location", IPP_TAG_TEXT)) != NULL)
    cupsdSetString(&pclass->location, attr->values[0].string.text);

  if ((attr = ippFindAttribute(con->request, "printer-geo-location", IPP_TAG_URI)) != NULL && !strncmp(attr->values[0].string.text, "geo:", 4))
    cupsdSetString(&pclass->geo_location, attr->values[0].string.text);

  if ((attr = ippFindAttribute(con->request, "printer-organization", IPP_TAG_TEXT)) != NULL)
    cupsdSetString(&pclass->organization, attr->values[0].string.text);

  if ((attr = ippFindAttribute(con->request, "printer-organizational-unit", IPP_TAG_TEXT)) != NULL)
    cupsdSetString(&pclass->organizational_unit, attr->values[0].string.text);

  if ((attr = ippFindAttribute(con->request, "printer-info",
                               IPP_TAG_TEXT)) != NULL)
    cupsdSetString(&pclass->info, attr->values[0].string.text);

  if ((attr = ippFindAttribute(con->request, "printer-is-accepting-jobs",
                               IPP_TAG_BOOLEAN)) != NULL &&
      attr->values[0].boolean != pclass->accepting)
  {
    cupsdLogMessage(CUPSD_LOG_INFO,
                    "Setting %s printer-is-accepting-jobs to %d (was %d.)",
                    pclass->name, attr->values[0].boolean, pclass->accepting);

    pclass->accepting = attr->values[0].boolean;

    cupsdAddEvent(CUPSD_EVENT_PRINTER_STATE, pclass, NULL, "%s accepting jobs.",
		  pclass->accepting ? "Now" : "No longer");
  }

  if ((attr = ippFindAttribute(con->request, "printer-is-shared", IPP_TAG_BOOLEAN)) != NULL)
  {
    if (pclass->type & CUPS_PRINTER_REMOTE)
    {
     /*
      * Cannot re-share remote printers.
      */

      send_ipp_status(con, IPP_BAD_REQUEST, _("Cannot change printer-is-shared for remote queues."));
      if (!modify)
	cupsdDeletePrinter(pclass, 0);

      return;
    }

    if (pclass->shared && !ippGetBoolean(attr, 0))
      cupsdDeregisterPrinter(pclass, 1);

    cupsdLogMessage(CUPSD_LOG_INFO,
                    "Setting %s printer-is-shared to %d (was %d.)",
                    pclass->name, attr->values[0].boolean, pclass->shared);

    pclass->shared = ippGetBoolean(attr, 0);
  }

  if ((attr = ippFindAttribute(con->request, "printer-state",
                               IPP_TAG_ENUM)) != NULL)
  {
    if (attr->values[0].integer != IPP_PRINTER_IDLE &&
        attr->values[0].integer != IPP_PRINTER_STOPPED)
    {
      send_ipp_status(con, IPP_BAD_REQUEST,
                      _("Attempt to set %s printer-state to bad value %d."),
                      pclass->name, attr->values[0].integer);
      if (!modify)
	cupsdDeletePrinter(pclass, 0);

      return;
    }

    cupsdLogMessage(CUPSD_LOG_INFO, "Setting %s printer-state to %d (was %d.)",
                    pclass->name, attr->values[0].integer, pclass->state);

    if (attr->values[0].integer == IPP_PRINTER_STOPPED)
      cupsdStopPrinter(pclass, 0);
    else
    {
      cupsdSetPrinterState(pclass, (ipp_pstate_t)(attr->values[0].integer), 0);
      need_restart_job = 1;
    }
  }
  if ((attr = ippFindAttribute(con->request, "printer-state-message",
                               IPP_TAG_TEXT)) != NULL)
  {
    strlcpy(pclass->state_message, attr->values[0].string.text,
            sizeof(pclass->state_message));

    cupsdAddEvent(CUPSD_EVENT_PRINTER_STATE, pclass, NULL, "%s",
                  pclass->state_message);
  }
  if ((attr = ippFindAttribute(con->request, "member-uris",
                               IPP_TAG_URI)) != NULL)
  {
   /*
    * Clear the printer array as needed...
    */

    need_restart_job = 1;

    if (pclass->num_printers > 0)
    {
      free(pclass->printers);
      pclass->num_printers = 0;
    }

   /*
    * Add each printer or class that is listed...
    */

    for (i = 0; i < attr->num_values; i ++)
    {
     /*
      * Search for the printer or class URI...
      */

      if (!cupsdValidateDest(attr->values[i].string.text, &dtype, &member))
      {
       /*
	* Bad URI...
	*/

	send_ipp_status(con, IPP_NOT_FOUND,
                	_("The printer or class does not exist."));
	if (!modify)
	  cupsdDeletePrinter(pclass, 0);

	return;
      }
      else if (dtype & CUPS_PRINTER_CLASS)
      {
        send_ipp_status(con, IPP_BAD_REQUEST,
			_("Nested classes are not allowed."));
	if (!modify)
	  cupsdDeletePrinter(pclass, 0);

        return;
      }

     /*
      * Add it to the class...
      */

      cupsdAddPrinterToClass(pclass, member);
    }
  }

  if (!set_printer_defaults(con, pclass))
  {
    if (!modify)
      cupsdDeletePrinter(pclass, 0);

    return;
  }

  if ((attr = ippFindAttribute(con->request, "auth-info-required",
                               IPP_TAG_KEYWORD)) != NULL)
    cupsdSetAuthInfoRequired(pclass, NULL, attr);

  pclass->config_time = time(NULL);

 /*
  * Update the printer class attributes and return...
  */

  cupsdSetPrinterAttrs(pclass);
  cupsdMarkDirty(CUPSD_DIRTY_CLASSES);

  if (need_restart_job && pclass->job)
  {
   /*
    * Reset the current job to a "pending" status...
    */

    cupsdSetJobState(pclass->job, IPP_JOB_PENDING, CUPSD_JOB_FORCE,
                     "Job restarted because the class was modified.");
  }

  cupsdMarkDirty(CUPSD_DIRTY_PRINTCAP);

  if (modify)
  {
    cupsdAddEvent(CUPSD_EVENT_PRINTER_MODIFIED,
		  pclass, NULL, "Class \"%s\" modified by \"%s\".",
		  pclass->name, get_username(con));

    cupsdLogMessage(CUPSD_LOG_INFO, "Class \"%s\" modified by \"%s\".",
                    pclass->name, get_username(con));
  }
  else
  {
    cupsdAddEvent(CUPSD_EVENT_PRINTER_ADDED,
		  pclass, NULL, "New class \"%s\" added by \"%s\".",
		  pclass->name, get_username(con));

    cupsdLogMessage(CUPSD_LOG_INFO, "New class \"%s\" added by \"%s\".",
                    pclass->name, get_username(con));
  }

  con->response->request.status.status_code = IPP_OK;
}
