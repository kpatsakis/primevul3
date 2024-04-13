add_file(cupsd_client_t *con,		/* I - Connection to client */
         cupsd_job_t    *job,		/* I - Job to add to */
         mime_type_t    *filetype,	/* I - Type of file */
	 int            compression)	/* I - Compression */
{
  mime_type_t	**filetypes;		/* New filetypes array... */
  int		*compressions;		/* New compressions array... */


  cupsdLogMessage(CUPSD_LOG_DEBUG2,
        	  "add_file(con=%p[%d], job=%d, filetype=%s/%s, "
		  "compression=%d)", con, con ? con->number : -1, job->id,
		  filetype->super, filetype->type, compression);

 /*
  * Add the file to the job...
  */

  if (job->num_files == 0)
  {
    compressions = (int *)malloc(sizeof(int));
    filetypes    = (mime_type_t **)malloc(sizeof(mime_type_t *));
  }
  else
  {
    compressions = (int *)realloc(job->compressions,
                                  (size_t)(job->num_files + 1) * sizeof(int));
    filetypes    = (mime_type_t **)realloc(job->filetypes,
                                           (size_t)(job->num_files + 1) *
					   sizeof(mime_type_t *));
  }

  if (compressions)
    job->compressions = compressions;

  if (filetypes)
    job->filetypes = filetypes;

  if (!compressions || !filetypes)
  {
    cupsdSetJobState(job, IPP_JOB_ABORTED, CUPSD_JOB_PURGE,
                     "Job aborted because the scheduler ran out of memory.");

    if (con)
      send_ipp_status(con, IPP_INTERNAL_ERROR,
		      _("Unable to allocate memory for file types."));

    return (-1);
  }

  job->compressions[job->num_files] = compression;
  job->filetypes[job->num_files]    = filetype;

  job->num_files ++;

  job->dirty = 1;
  cupsdMarkDirty(CUPSD_DIRTY_JOBS);

  return (0);
}
