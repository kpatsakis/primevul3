cancel_subscription(
    cupsd_client_t *con,		/* I - Client connection */
    int            sub_id)		/* I - Subscription ID */
{
  http_status_t		status;		/* Policy status */
  cupsd_subscription_t	*sub;		/* Subscription */


  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "cancel_subscription(con=%p[%d], sub_id=%d)",
                  con, con->number, sub_id);

 /*
  * Is the subscription ID valid?
  */

  if ((sub = cupsdFindSubscription(sub_id)) == NULL)
  {
   /*
    * Bad subscription ID...
    */

    send_ipp_status(con, IPP_NOT_FOUND,
                    _("Subscription #%d does not exist."), sub_id);
    return;
  }

 /*
  * Check policy...
  */

  if ((status = cupsdCheckPolicy(sub->dest ? sub->dest->op_policy_ptr :
                                             DefaultPolicyPtr,
                                 con, sub->owner)) != HTTP_OK)
  {
    send_http_error(con, status, sub->dest);
    return;
  }

 /*
  * Cancel the subscription...
  */

  cupsdDeleteSubscription(sub, 1);

  con->response->request.status.status_code = IPP_OK;
}
