cancel_all_jobs(cupsd_client_t  *con,	/* I - Client connection */
	        ipp_attribute_t *uri)	/* I - Job or Printer URI */
{
  int		i;			/* Looping var */
  http_status_t	status;			/* Policy status */
  cups_ptype_t	dtype;			/* Destination type */
  char		scheme[HTTP_MAX_URI],	/* Scheme portion of URI */
		userpass[HTTP_MAX_URI],	/* Username portion of URI */
		hostname[HTTP_MAX_URI],	/* Host portion of URI */
		resource[HTTP_MAX_URI];	/* Resource portion of URI */
  int		port;			/* Port portion of URI */
  ipp_attribute_t *attr;		/* Attribute in request */
  const char	*username = NULL;	/* Username */
  cupsd_jobaction_t purge = CUPSD_JOB_DEFAULT;
					/* Purge? */
  cupsd_printer_t *printer;		/* Printer */
  ipp_attribute_t *job_ids;		/* job-ids attribute */
  cupsd_job_t	*job;			/* Job */


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "cancel_all_jobs(%p[%d], %s)", con,
                  con->number, uri->values[0].string.text);

 /*
  * Get the jobs to cancel/purge...
  */

  switch (con->request->request.op.operation_id)
  {
    case IPP_PURGE_JOBS :
       /*
	* Get the username (if any) for the jobs we want to cancel (only if
	* "my-jobs" is specified...
	*/

        if ((attr = ippFindAttribute(con->request, "my-jobs",
                                     IPP_TAG_BOOLEAN)) != NULL &&
            attr->values[0].boolean)
	{
	  if ((attr = ippFindAttribute(con->request, "requesting-user-name",
				       IPP_TAG_NAME)) != NULL)
	    username = attr->values[0].string.text;
	  else
	  {
	    send_ipp_status(con, IPP_BAD_REQUEST,
			    _("Missing requesting-user-name attribute."));
	    return;
	  }
	}

       /*
	* Look for the "purge-jobs" attribute...
	*/

	if ((attr = ippFindAttribute(con->request, "purge-jobs",
				     IPP_TAG_BOOLEAN)) != NULL)
	  purge = attr->values[0].boolean ? CUPSD_JOB_PURGE : CUPSD_JOB_DEFAULT;
	else
	  purge = CUPSD_JOB_PURGE;
	break;

    case IPP_CANCEL_MY_JOBS :
        if (con->username[0])
          username = con->username;
        else if ((attr = ippFindAttribute(con->request, "requesting-user-name",
					  IPP_TAG_NAME)) != NULL)
          username = attr->values[0].string.text;
        else
        {
	  send_ipp_status(con, IPP_BAD_REQUEST,
			  _("Missing requesting-user-name attribute."));
	  return;
        }

    default :
        break;
  }

  job_ids = ippFindAttribute(con->request, "job-ids", IPP_TAG_INTEGER);

 /*
  * See if we have a printer URI...
  */

  if (strcmp(uri->name, "printer-uri"))
  {
    send_ipp_status(con, IPP_BAD_REQUEST,
                    _("The printer-uri attribute is required."));
    return;
  }

 /*
  * And if the destination is valid...
  */

  if (!cupsdValidateDest(uri->values[0].string.text, &dtype, &printer))
  {
   /*
    * Bad URI?
    */

    httpSeparateURI(HTTP_URI_CODING_ALL, uri->values[0].string.text,
                    scheme, sizeof(scheme), userpass, sizeof(userpass),
		    hostname, sizeof(hostname), &port,
		    resource, sizeof(resource));

    if ((!strncmp(resource, "/printers/", 10) && resource[10]) ||
        (!strncmp(resource, "/classes/", 9) && resource[9]))
    {
      send_ipp_status(con, IPP_NOT_FOUND,
                      _("The printer or class does not exist."));
      return;
    }

   /*
    * Check policy...
    */

    if ((status = cupsdCheckPolicy(DefaultPolicyPtr, con, NULL)) != HTTP_OK)
    {
      send_http_error(con, status, NULL);
      return;
    }

    if (job_ids)
    {
      for (i = 0; i < job_ids->num_values; i ++)
      {
	if ((job = cupsdFindJob(job_ids->values[i].integer)) == NULL)
	  break;

        if (con->request->request.op.operation_id == IPP_CANCEL_MY_JOBS &&
            _cups_strcasecmp(job->username, username))
          break;
      }

      if (i < job_ids->num_values)
      {
	send_ipp_status(con, IPP_NOT_FOUND, _("Job #%d does not exist."),
			job_ids->values[i].integer);
	return;
      }

      for (i = 0; i < job_ids->num_values; i ++)
      {
	job = cupsdFindJob(job_ids->values[i].integer);

	cupsdSetJobState(job, IPP_JOB_CANCELED, purge,
	                 purge == CUPSD_JOB_PURGE ? "Job purged by user." :
	                                            "Job canceled by user.");
      }

      cupsdLogMessage(CUPSD_LOG_INFO, "Selected jobs were %s by \"%s\".",
		      purge == CUPSD_JOB_PURGE ? "purged" : "canceled",
		      get_username(con));
    }
    else
    {
     /*
      * Cancel all jobs on all printers...
      */

      cupsdCancelJobs(NULL, username, purge);

      cupsdLogMessage(CUPSD_LOG_INFO, "All jobs were %s by \"%s\".",
		      purge == CUPSD_JOB_PURGE ? "purged" : "canceled",
		      get_username(con));
    }
  }
  else
  {
   /*
    * Check policy...
    */

    if ((status = cupsdCheckPolicy(printer->op_policy_ptr, con,
                                   NULL)) != HTTP_OK)
    {
      send_http_error(con, status, printer);
      return;
    }

    if (job_ids)
    {
      for (i = 0; i < job_ids->num_values; i ++)
      {
	if ((job = cupsdFindJob(job_ids->values[i].integer)) == NULL ||
	    _cups_strcasecmp(job->dest, printer->name))
	  break;

        if (con->request->request.op.operation_id == IPP_CANCEL_MY_JOBS &&
            _cups_strcasecmp(job->username, username))
          break;
      }

      if (i < job_ids->num_values)
      {
	send_ipp_status(con, IPP_NOT_FOUND, _("Job #%d does not exist."),
			job_ids->values[i].integer);
	return;
      }

      for (i = 0; i < job_ids->num_values; i ++)
      {
	job = cupsdFindJob(job_ids->values[i].integer);

	cupsdSetJobState(job, IPP_JOB_CANCELED, purge,
	                 purge == CUPSD_JOB_PURGE ? "Job purged by user." :
	                                            "Job canceled by user.");
      }

      cupsdLogMessage(CUPSD_LOG_INFO, "Selected jobs were %s by \"%s\".",
		      purge == CUPSD_JOB_PURGE ? "purged" : "canceled",
		      get_username(con));
    }
    else
    {
     /*
      * Cancel all of the jobs on the named printer...
      */

      cupsdCancelJobs(printer->name, username, purge);

      cupsdLogMessage(CUPSD_LOG_INFO, "All jobs on \"%s\" were %s by \"%s\".",
		      printer->name,
		      purge == CUPSD_JOB_PURGE ? "purged" : "canceled",
		      get_username(con));
    }
  }

  con->response->request.status.status_code = IPP_OK;

  cupsdCheckJobs();
}
