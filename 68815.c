check_quotas(cupsd_client_t  *con,	/* I - Client connection */
             cupsd_printer_t *p)	/* I - Printer or class */
{
  char		username[33],		/* Username */
		*name;			/* Current user name */
  cupsd_quota_t	*q;			/* Quota data */
#ifdef HAVE_MBR_UID_TO_UUID
 /*
  * Use Apple membership APIs which require that all names represent
  * valid user account or group records accessible by the server.
  */

  uuid_t	usr_uuid;		/* UUID for job requesting user  */
  uuid_t	usr2_uuid;		/* UUID for ACL user name entry  */
  uuid_t	grp_uuid;		/* UUID for ACL group name entry */
  int		mbr_err;		/* Error from membership function */
  int		is_member;		/* Is this user a member? */
#else
 /*
  * Use standard POSIX APIs for checking users and groups...
  */

  struct passwd	*pw;			/* User password data */
#endif /* HAVE_MBR_UID_TO_UUID */


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "check_quotas(%p[%d], %p[%s])",
                  con, con->number, p, p->name);

 /*
  * Figure out who is printing...
  */

  strlcpy(username, get_username(con), sizeof(username));

  if ((name = strchr(username, '@')) != NULL)
    *name = '\0';			/* Strip @REALM */

 /*
  * Check global active job limits for printers and users...
  */

  if (MaxJobsPerPrinter)
  {
   /*
    * Check if there are too many pending jobs on this printer...
    */

    if (cupsdGetPrinterJobCount(p->name) >= MaxJobsPerPrinter)
    {
      cupsdLogMessage(CUPSD_LOG_INFO, "Too many jobs for printer \"%s\"...",
                      p->name);
      return (-1);
    }
  }

  if (MaxJobsPerUser)
  {
   /*
    * Check if there are too many pending jobs for this user...
    */

    if (cupsdGetUserJobCount(username) >= MaxJobsPerUser)
    {
      cupsdLogMessage(CUPSD_LOG_INFO, "Too many jobs for user \"%s\"...",
                      username);
      return (-1);
    }
  }

 /*
  * Check against users...
  */

  if (cupsArrayCount(p->users) == 0 && p->k_limit == 0 && p->page_limit == 0)
    return (1);

  if (cupsArrayCount(p->users))
  {
#ifdef HAVE_MBR_UID_TO_UUID
   /*
    * Get UUID for job requesting user...
    */

    if (mbr_user_name_to_uuid((char *)username, usr_uuid))
    {
     /*
      * Unknown user...
      */

      cupsdLogMessage(CUPSD_LOG_DEBUG,
		      "check_quotas: UUID lookup failed for user \"%s\"",
		      username);
      cupsdLogMessage(CUPSD_LOG_INFO,
		      "Denying user \"%s\" access to printer \"%s\" "
		      "(unknown user)...",
		      username, p->name);
      return (0);
    }
#else
   /*
    * Get UID and GID of requesting user...
    */

    pw = getpwnam(username);
    endpwent();
#endif /* HAVE_MBR_UID_TO_UUID */

    for (name = (char *)cupsArrayFirst(p->users);
         name;
	 name = (char *)cupsArrayNext(p->users))
      if (name[0] == '@')
      {
       /*
        * Check group membership...
	*/

#ifdef HAVE_MBR_UID_TO_UUID
        if (name[1] == '#')
	{
	  if (uuid_parse(name + 2, grp_uuid))
	    uuid_clear(grp_uuid);
	}
	else if ((mbr_err = mbr_group_name_to_uuid(name + 1, grp_uuid)) != 0)
	{
	 /*
	  * Invalid ACL entries are ignored for matching; just record a
	  * warning in the log...
	  */

	  cupsdLogMessage(CUPSD_LOG_DEBUG,
	                  "check_quotas: UUID lookup failed for ACL entry "
			  "\"%s\" (err=%d)", name, mbr_err);
	  cupsdLogMessage(CUPSD_LOG_WARN,
	                  "Access control entry \"%s\" not a valid group name; "
			  "entry ignored", name);
	}

	if ((mbr_err = mbr_check_membership(usr_uuid, grp_uuid,
					    &is_member)) != 0)
	{
	 /*
	  * At this point, there should be no errors, but check anyways...
	  */

	  cupsdLogMessage(CUPSD_LOG_DEBUG,
			  "check_quotas: group \"%s\" membership check "
			  "failed (err=%d)", name + 1, mbr_err);
	  is_member = 0;
	}

       /*
	* Stop if we found a match...
	*/

	if (is_member)
	  break;

#else
        if (cupsdCheckGroup(username, pw, name + 1))
	  break;
#endif /* HAVE_MBR_UID_TO_UUID */
      }
#ifdef HAVE_MBR_UID_TO_UUID
      else
      {
        if (name[0] == '#')
	{
	  if (uuid_parse(name + 1, usr2_uuid))
	    uuid_clear(usr2_uuid);
        }
        else if ((mbr_err = mbr_user_name_to_uuid(name, usr2_uuid)) != 0)
    	{
	 /*
	  * Invalid ACL entries are ignored for matching; just record a
	  * warning in the log...
	  */

          cupsdLogMessage(CUPSD_LOG_DEBUG,
	                  "check_quotas: UUID lookup failed for ACL entry "
			  "\"%s\" (err=%d)", name, mbr_err);
          cupsdLogMessage(CUPSD_LOG_WARN,
	                  "Access control entry \"%s\" not a valid user name; "
			  "entry ignored", name);
	}

	if (!uuid_compare(usr_uuid, usr2_uuid))
	  break;
      }
#else
      else if (!_cups_strcasecmp(username, name))
	break;
#endif /* HAVE_MBR_UID_TO_UUID */

    if ((name != NULL) == p->deny_users)
    {
      cupsdLogMessage(CUPSD_LOG_INFO,
                      "Denying user \"%s\" access to printer \"%s\"...",
        	      username, p->name);
      return (0);
    }
  }

 /*
  * Check quotas...
  */

  if (p->k_limit || p->page_limit)
  {
    if ((q = cupsdUpdateQuota(p, username, 0, 0)) == NULL)
    {
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to allocate quota data for user \"%s\"",
                      username);
      return (-1);
    }

    if ((q->k_count >= p->k_limit && p->k_limit) ||
        (q->page_count >= p->page_limit && p->page_limit))
    {
      cupsdLogMessage(CUPSD_LOG_INFO, "User \"%s\" is over the quota limit...",
                      username);
      return (-1);
    }
  }

 /*
  * If we have gotten this far, we're done!
  */

  return (1);
}
