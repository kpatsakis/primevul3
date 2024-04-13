cancel_job(cupsd_client_t  *con,	/* I - Client connection */
	   ipp_attribute_t *uri)	/* I - Job or Printer URI */
{
  ipp_attribute_t *attr;		/* Current attribute */
  int		jobid;			/* Job ID */
  char		scheme[HTTP_MAX_URI],	/* Scheme portion of URI */
		username[HTTP_MAX_URI],	/* Username portion of URI */
		host[HTTP_MAX_URI],	/* Host portion of URI */
		resource[HTTP_MAX_URI];	/* Resource portion of URI */
  int		port;			/* Port portion of URI */
  cupsd_job_t	*job;			/* Job information */
  cups_ptype_t	dtype;			/* Destination type (printer/class) */
  cupsd_printer_t *printer;		/* Printer data */
  cupsd_jobaction_t purge;		/* Purge the job? */


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "cancel_job(%p[%d], %s)", con,
                  con->number, uri->values[0].string.text);

 /*
  * See if we have a job URI or a printer URI...
  */

  if (!strcmp(uri->name, "printer-uri"))
  {
   /*
    * Got a printer URI; see if we also have a job-id attribute...
    */

    if ((attr = ippFindAttribute(con->request, "job-id",
                                 IPP_TAG_INTEGER)) == NULL)
    {
      send_ipp_status(con, IPP_BAD_REQUEST,
                      _("Got a printer-uri attribute but no job-id."));
      return;
    }

    if ((jobid = attr->values[0].integer) == 0)
    {
     /*
      * Find the current job on the specified printer...
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
      * See if there are any pending jobs...
      */

      for (job = (cupsd_job_t *)cupsArrayFirst(ActiveJobs);
	   job;
	   job = (cupsd_job_t *)cupsArrayNext(ActiveJobs))
	if (job->state_value <= IPP_JOB_PROCESSING &&
	    !_cups_strcasecmp(job->dest, printer->name))
	  break;

      if (job)
	jobid = job->id;
      else
      {
       /*
        * No, try stopped jobs...
	*/

	for (job = (cupsd_job_t *)cupsArrayFirst(ActiveJobs);
	     job;
	     job = (cupsd_job_t *)cupsArrayNext(ActiveJobs))
	  if (job->state_value == IPP_JOB_STOPPED &&
	      !_cups_strcasecmp(job->dest, printer->name))
	    break;

	if (job)
	  jobid = job->id;
	else
	{
	  send_ipp_status(con, IPP_NOT_POSSIBLE, _("No active jobs on %s."),
			  printer->name);
	  return;
	}
      }
    }
  }
  else
  {
   /*
    * Got a job URI; parse it to get the job ID...
    */

    httpSeparateURI(HTTP_URI_CODING_ALL, uri->values[0].string.text, scheme,
                    sizeof(scheme), username, sizeof(username), host,
		    sizeof(host), &port, resource, sizeof(resource));

    if (strncmp(resource, "/jobs/", 6))
    {
     /*
      * Not a valid URI!
      */

      send_ipp_status(con, IPP_BAD_REQUEST, _("Bad job-uri \"%s\"."),
                      uri->values[0].string.text);
      return;
    }

    jobid = atoi(resource + 6);
  }

 /*
  * Look for the "purge-job" attribute...
  */

  if ((attr = ippFindAttribute(con->request, "purge-job",
                               IPP_TAG_BOOLEAN)) != NULL)
    purge = attr->values[0].boolean ? CUPSD_JOB_PURGE : CUPSD_JOB_DEFAULT;
  else
    purge = CUPSD_JOB_DEFAULT;

 /*
  * See if the job exists...
  */

  if ((job = cupsdFindJob(jobid)) == NULL)
  {
   /*
    * Nope - return a "not found" error...
    */

    send_ipp_status(con, IPP_NOT_FOUND, _("Job #%d does not exist."), jobid);
    return;
  }

 /*
  * See if the job is owned by the requesting user...
  */

  if (!validate_user(job, con, job->username, username, sizeof(username)))
  {
    send_http_error(con, con->username[0] ? HTTP_FORBIDDEN : HTTP_UNAUTHORIZED,
                    cupsdFindDest(job->dest));
    return;
  }

 /*
  * See if the job is already completed, canceled, or aborted; if so,
  * we can't cancel...
  */

  if (job->state_value >= IPP_JOB_CANCELED && purge != CUPSD_JOB_PURGE)
  {
    switch (job->state_value)
    {
      case IPP_JOB_CANCELED :
	  send_ipp_status(con, IPP_NOT_POSSIBLE,
                	  _("Job #%d is already canceled - can\'t cancel."),
			  jobid);
          break;

      case IPP_JOB_ABORTED :
	  send_ipp_status(con, IPP_NOT_POSSIBLE,
                	  _("Job #%d is already aborted - can\'t cancel."),
			  jobid);
          break;

      default :
	  send_ipp_status(con, IPP_NOT_POSSIBLE,
                	  _("Job #%d is already completed - can\'t cancel."),
			  jobid);
          break;
    }

    return;
  }

 /*
  * Cancel the job and return...
  */

  cupsdSetJobState(job, IPP_JOB_CANCELED, purge,
                   purge == CUPSD_JOB_PURGE ? "Job purged by \"%s\"" :
		                              "Job canceled by \"%s\"",
		   username);
  cupsdCheckJobs();

  if (purge == CUPSD_JOB_PURGE)
    cupsdLogMessage(CUPSD_LOG_INFO, "[Job %d] Purged by \"%s\".", jobid,
		    username);
  else
    cupsdLogMessage(CUPSD_LOG_INFO, "[Job %d] Canceled by \"%s\".", jobid,
		    username);

  con->response->request.status.status_code = IPP_OK;
}
