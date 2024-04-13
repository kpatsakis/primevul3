am_saml_response_status_str(request_rec *r, LassoNode *node)
{
    LassoSamlp2StatusResponse *response = (LassoSamlp2StatusResponse*)node;
    LassoSamlp2Status *status = NULL;
    const char *status_code1 = NULL;
    const char *status_code2 = NULL;

    if (!LASSO_IS_SAMLP2_STATUS_RESPONSE(response)) {
        return apr_psprintf(r->pool,
                            "error, expected LassoSamlp2StatusResponse "
                            "but got %s",
                            lasso_node_get_name((LassoNode*)response));
    }

    status = response->Status;
    if (status == NULL                  ||
        !LASSO_IS_SAMLP2_STATUS(status) ||
        status->StatusCode == NULL      ||
        status->StatusCode->Value == NULL) {
        return apr_psprintf(r->pool, "Status missing");

    }

    status_code1 = status->StatusCode->Value;
    if (status->StatusCode->StatusCode) {
        status_code2 = status->StatusCode->StatusCode->Value;
    }

    return apr_psprintf(r->pool,
                        "StatusCode1=\"%s\", StatusCode2=\"%s\", "
                        "StatusMessage=\"%s\"",
                        status_code1, status_code2, status->StatusMessage);
}
