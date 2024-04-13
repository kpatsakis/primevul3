apply_printer_defaults(
    cupsd_printer_t *printer,		/* I - Printer */
    cupsd_job_t     *job)		/* I - Job */
{
  int		i,			/* Looping var */
		num_options;		/* Number of default options */
  cups_option_t	*options,		/* Default options */
		*option;		/* Current option */


  cupsdLogJob(job, CUPSD_LOG_DEBUG, "Applying default options...");

 /*
  * Collect all of the default options and add the missing ones to the
  * job object...
  */

  for (i = printer->num_options, num_options = 0, options = NULL,
           option = printer->options;
       i > 0;
       i --, option ++)
    if (!ippFindAttribute(job->attrs, option->name, IPP_TAG_ZERO))
    {
      if (!strcmp(option->name, "print-quality") && ippFindAttribute(job->attrs, "cupsPrintQuality", IPP_TAG_NAME))
        continue;                     /* Don't override cupsPrintQuality */

      cupsdLogJob(job, CUPSD_LOG_DEBUG, "Adding default %s=%s", option->name, option->value);

      num_options = cupsAddOption(option->name, option->value, num_options, &options);
    }

 /*
  * Encode these options as attributes in the job object...
  */

  cupsEncodeOptions2(job->attrs, num_options, options, IPP_TAG_JOB);
  cupsFreeOptions(num_options, options);
}
