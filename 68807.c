add_job_subscriptions(
    cupsd_client_t *con,		/* I - Client connection */
    cupsd_job_t    *job)		/* I - Newly created job */
{
  int			i;		/* Looping var */
  ipp_attribute_t	*prev,		/* Previous attribute */
			*next,		/* Next attribute */
			*attr;		/* Current attribute */
  cupsd_subscription_t	*sub;		/* Subscription object */
  const char		*recipient,	/* notify-recipient-uri */
			*pullmethod;	/* notify-pull-method */
  ipp_attribute_t	*user_data;	/* notify-user-data */
  int			interval;	/* notify-time-interval */
  unsigned		mask;		/* notify-events */


 /*
  * Find the first subscription group attribute; return if we have
  * none...
  */

  for (attr = job->attrs->attrs; attr; attr = attr->next)
    if (attr->group_tag == IPP_TAG_SUBSCRIPTION)
      break;

  if (!attr)
    return;

 /*
  * Process the subscription attributes in the request...
  */

  while (attr)
  {
    recipient = NULL;
    pullmethod = NULL;
    user_data  = NULL;
    interval   = 0;
    mask       = CUPSD_EVENT_NONE;

    while (attr && attr->group_tag != IPP_TAG_ZERO)
    {
      if (!strcmp(attr->name, "notify-recipient-uri") &&
          attr->value_tag == IPP_TAG_URI)
      {
       /*
        * Validate the recipient scheme against the ServerBin/notifier
	* directory...
	*/

	char	notifier[1024],		/* Notifier filename */
		scheme[HTTP_MAX_URI],	/* Scheme portion of URI */
		userpass[HTTP_MAX_URI],	/* Username portion of URI */
		host[HTTP_MAX_URI],	/* Host portion of URI */
		resource[HTTP_MAX_URI];	/* Resource portion of URI */
        int	port;			/* Port portion of URI */


        recipient = attr->values[0].string.text;

	if (httpSeparateURI(HTTP_URI_CODING_ALL, recipient,
	                    scheme, sizeof(scheme), userpass, sizeof(userpass),
			    host, sizeof(host), &port,
			    resource, sizeof(resource)) < HTTP_URI_OK)
        {
          send_ipp_status(con, IPP_NOT_POSSIBLE,
	                  _("Bad notify-recipient-uri \"%s\"."), recipient);
	  ippAddInteger(con->response, IPP_TAG_SUBSCRIPTION, IPP_TAG_ENUM,
	                "notify-status-code", IPP_URI_SCHEME);
	  return;
	}

        snprintf(notifier, sizeof(notifier), "%s/notifier/%s", ServerBin,
	         scheme);
        if (access(notifier, X_OK))
	{
          send_ipp_status(con, IPP_NOT_POSSIBLE,
	                  _("notify-recipient-uri URI \"%s\" uses unknown "
			    "scheme."), recipient);
	  ippAddInteger(con->response, IPP_TAG_SUBSCRIPTION, IPP_TAG_ENUM,
	                "notify-status-code", IPP_URI_SCHEME);
	  return;
	}

        if (!strcmp(scheme, "rss") && !check_rss_recipient(recipient))
	{
          send_ipp_status(con, IPP_NOT_POSSIBLE,
	                  _("notify-recipient-uri URI \"%s\" is already used."),
			  recipient);
	  ippAddInteger(con->response, IPP_TAG_SUBSCRIPTION, IPP_TAG_ENUM,
	                "notify-status-code", IPP_ATTRIBUTES);
	  return;
	}
      }
      else if (!strcmp(attr->name, "notify-pull-method") &&
               attr->value_tag == IPP_TAG_KEYWORD)
      {
        pullmethod = attr->values[0].string.text;

        if (strcmp(pullmethod, "ippget"))
	{
          send_ipp_status(con, IPP_NOT_POSSIBLE,
	                  _("Bad notify-pull-method \"%s\"."), pullmethod);
	  ippAddInteger(con->response, IPP_TAG_SUBSCRIPTION, IPP_TAG_ENUM,
	                "notify-status-code", IPP_ATTRIBUTES);
	  return;
	}
      }
      else if (!strcmp(attr->name, "notify-charset") &&
               attr->value_tag == IPP_TAG_CHARSET &&
	       strcmp(attr->values[0].string.text, "us-ascii") &&
	       strcmp(attr->values[0].string.text, "utf-8"))
      {
        send_ipp_status(con, IPP_CHARSET,
	                _("Character set \"%s\" not supported."),
			attr->values[0].string.text);
	return;
      }
      else if (!strcmp(attr->name, "notify-natural-language") &&
               (attr->value_tag != IPP_TAG_LANGUAGE ||
	        strcmp(attr->values[0].string.text, DefaultLanguage)))
      {
        send_ipp_status(con, IPP_CHARSET,
	                _("Language \"%s\" not supported."),
			attr->values[0].string.text);
	return;
      }
      else if (!strcmp(attr->name, "notify-user-data") &&
               attr->value_tag == IPP_TAG_STRING)
      {
        if (attr->num_values > 1 || attr->values[0].unknown.length > 63)
	{
          send_ipp_status(con, IPP_REQUEST_VALUE,
	                  _("The notify-user-data value is too large "
			    "(%d > 63 octets)."),
			  attr->values[0].unknown.length);
	  return;
	}

        user_data = attr;
      }
      else if (!strcmp(attr->name, "notify-events") &&
               attr->value_tag == IPP_TAG_KEYWORD)
      {
        for (i = 0; i < attr->num_values; i ++)
	  mask |= cupsdEventValue(attr->values[i].string.text);
      }
      else if (!strcmp(attr->name, "notify-lease-duration"))
      {
        send_ipp_status(con, IPP_BAD_REQUEST,
	                _("The notify-lease-duration attribute cannot be "
			  "used with job subscriptions."));
	return;
      }
      else if (!strcmp(attr->name, "notify-time-interval") &&
               attr->value_tag == IPP_TAG_INTEGER)
        interval = attr->values[0].integer;

      attr = attr->next;
    }

    if (!recipient && !pullmethod)
      break;

    if (mask == CUPSD_EVENT_NONE)
      mask = CUPSD_EVENT_JOB_COMPLETED;

    if ((sub = cupsdAddSubscription(mask, cupsdFindDest(job->dest), job,
                                    recipient, 0)) != NULL)
    {
      sub->interval = interval;

      cupsdSetString(&sub->owner, job->username);

      if (user_data)
      {
	sub->user_data_len = user_data->values[0].unknown.length;
	memcpy(sub->user_data, user_data->values[0].unknown.data,
	       (size_t)sub->user_data_len);
      }

      ippAddSeparator(con->response);
      ippAddInteger(con->response, IPP_TAG_SUBSCRIPTION, IPP_TAG_INTEGER,
		    "notify-subscription-id", sub->id);

      cupsdLogMessage(CUPSD_LOG_DEBUG, "Added subscription %d for job %d",
                      sub->id, job->id);
    }

    if (attr)
      attr = attr->next;
  }

  cupsdMarkDirty(CUPSD_DIRTY_SUBSCRIPTIONS);

 /*
  * Remove all of the subscription attributes from the job request...
  *
  * TODO: Optimize this since subscription groups have to come at the
  * end of the request...
  */

  for (attr = job->attrs->attrs, prev = NULL; attr; attr = next)
  {
    next = attr->next;

    if (attr->group_tag == IPP_TAG_SUBSCRIPTION ||
        attr->group_tag == IPP_TAG_ZERO)
    {
     /*
      * Free and remove this attribute...
      */

      ippDeleteAttribute(NULL, attr);

      if (prev)
        prev->next = next;
      else
        job->attrs->attrs = next;
    }
    else
      prev = attr;
  }

  job->attrs->last    = prev;
  job->attrs->current = prev;
}
