copy_attrs(ipp_t        *to,		/* I - Destination request */
           ipp_t        *from,		/* I - Source request */
           cups_array_t *ra,		/* I - Requested attributes */
	   ipp_tag_t    group,		/* I - Group to copy */
	   int          quickcopy,	/* I - Do a quick copy? */
	   cups_array_t *exclude)	/* I - Attributes to exclude? */
{
  ipp_attribute_t	*fromattr;	/* Source attribute */


  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "copy_attrs(to=%p, from=%p, ra=%p, group=%x, quickcopy=%d)",
		  to, from, ra, group, quickcopy);

  if (!to || !from)
    return;

  for (fromattr = from->attrs; fromattr; fromattr = fromattr->next)
  {
   /*
    * Filter attributes as needed...
    */

    if ((group != IPP_TAG_ZERO && fromattr->group_tag != group &&
         fromattr->group_tag != IPP_TAG_ZERO) || !fromattr->name)
      continue;

    if (!strcmp(fromattr->name, "document-password") ||
        !strcmp(fromattr->name, "job-authorization-uri") ||
        !strcmp(fromattr->name, "job-password") ||
        !strcmp(fromattr->name, "job-password-encryption") ||
        !strcmp(fromattr->name, "job-printer-uri"))
      continue;

    if (exclude &&
        (cupsArrayFind(exclude, fromattr->name) ||
	 cupsArrayFind(exclude, "all")))
    {
     /*
      * We need to exclude this attribute for security reasons; we require the
      * job-id attribute regardless of the security settings for IPP
      * conformance.
      *
      * The job-printer-uri attribute is handled by copy_job_attrs().
      *
      * Subscription attribute security is handled by copy_subscription_attrs().
      */

      if (strcmp(fromattr->name, "job-id"))
        continue;
    }

    if (!ra || cupsArrayFind(ra, fromattr->name))
    {
     /*
      * Don't send collection attributes by default to IPP/1.x clients
      * since many do not support collections.  Also don't send
      * media-col-database unless specifically requested by the client.
      */

      if (fromattr->value_tag == IPP_TAG_BEGIN_COLLECTION &&
          !ra &&
	  (to->request.status.version[0] == 1 ||
	   !strcmp(fromattr->name, "media-col-database")))
	continue;

      ippCopyAttribute(to, fromattr, quickcopy);
    }
  }
}
