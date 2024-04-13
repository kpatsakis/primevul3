add_job_uuid(cupsd_job_t *job)		/* I - Job */
{
  char			uuid[64];	/* job-uuid string */


 /*
  * Add a job-uuid attribute if none exists...
  */

  if (!ippFindAttribute(job->attrs, "job-uuid", IPP_TAG_URI))
    ippAddString(job->attrs, IPP_TAG_JOB, IPP_TAG_URI, "job-uuid", NULL,
		 httpAssembleUUID(ServerName, RemotePort, job->dest, job->id,
		                  uuid, sizeof(uuid)));
}
