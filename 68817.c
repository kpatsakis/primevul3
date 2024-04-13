close_job(cupsd_client_t  *con,		/* I - Client connection */
          ipp_attribute_t *uri)		/* I - Printer URI */
{
  cupsd_job_t		*job;		/* Job */
  ipp_attribute_t	*attr;		/* Attribute */
  char			job_uri[HTTP_MAX_URI],
					/* Job URI */
			username[256];	/* User name */


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "close_job(%p[%d], %s)", con,
                  con->number, uri->values[0].string.text);

 /*
  * See if we have a job URI or a printer URI...
  */

  if (strcmp(uri->name, "printer-uri"))
  {
   /*
    * job-uri is not supported by Close-Job!
    */

    send_ipp_status(con, IPP_BAD_REQUEST,
		    _("Close-Job doesn't support the job-uri attribute."));
    return;
  }

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

  if ((job = cupsdFindJob(attr->values[0].integer)) == NULL)
  {
   /*
    * Nope - return a "not found" error...
    */

    send_ipp_status(con, IPP_NOT_FOUND, _("Job #%d does not exist."),
                    attr->values[0].integer);
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
  * Add any ending sheet...
  */

  if (cupsdTimeoutJob(job))
    return;

  if (job->state_value == IPP_JOB_STOPPED)
  {
    job->state->values[0].integer = IPP_JOB_PENDING;
    job->state_value              = IPP_JOB_PENDING;
  }
  else if (job->state_value == IPP_JOB_HELD)
  {
    if ((attr = ippFindAttribute(job->attrs, "job-hold-until",
				 IPP_TAG_KEYWORD)) == NULL)
      attr = ippFindAttribute(job->attrs, "job-hold-until", IPP_TAG_NAME);

    if (!attr || !strcmp(attr->values[0].string.text, "no-hold"))
    {
      job->state->values[0].integer = IPP_JOB_PENDING;
      job->state_value              = IPP_JOB_PENDING;
    }
  }

  job->dirty = 1;
  cupsdMarkDirty(CUPSD_DIRTY_JOBS);

 /*
  * Fill in the response info...
  */

  httpAssembleURIf(HTTP_URI_CODING_ALL, job_uri, sizeof(job_uri), "ipp", NULL,
                   con->clientname, con->clientport, "/jobs/%d", job->id);
  ippAddString(con->response, IPP_TAG_JOB, IPP_TAG_URI, "job-uri", NULL,
               job_uri);

  ippAddInteger(con->response, IPP_TAG_JOB, IPP_TAG_INTEGER, "job-id", job->id);

  ippAddInteger(con->response, IPP_TAG_JOB, IPP_TAG_ENUM, "job-state",
                job->state_value);

  con->response->request.status.status_code = IPP_OK;

 /*
  * Start the job if necessary...
  */

  cupsdCheckJobs();
}
