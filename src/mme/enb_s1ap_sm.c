#define TRACE_MODULE _enb_s1_sm

#include "core_debug.h"

#include "event.h"

#include "s1ap_build.h"
#include "s1ap_handler.h"

void enb_s1ap_state_initial(enb_s1ap_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);

    FSM_TRAN(s, &enb_s1ap_state_operational);
}

void enb_s1ap_state_final(enb_s1ap_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);
}

void enb_s1ap_state_operational(enb_s1ap_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    enb_ctx_t *enb = s->ctx;
    d_assert(enb, return, "Null param");

    mme_sm_trace(1, e);

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            break;
        }
        case FSM_EXIT_SIG:
        {
            break;
        }
        case EVT_MSG_ENB_S1AP:
        {
            s1ap_message_t message;
            status_t rv;
            pkbuf_t *recvbuf = (pkbuf_t *)event_get_param2(e);
            d_assert(recvbuf, break, "Null param");

            rv = s1ap_decode_pdu(&message, recvbuf);
            if (rv != CORE_OK) 
            {
                d_error("Can't parse S1AP_PDU");
                break;
            }

            switch(message.direction)
            {
                case S1AP_PDU_PR_initiatingMessage :
                {
                    switch(message.procedureCode)
                    {
                        case S1ap_ProcedureCode_id_S1Setup :
                        {
                            s1ap_handle_s1_setup_request(enb, &message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_initialUEMessage :
                        {
                            s1ap_handle_initial_ue_message(enb, &message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_uplinkNASTransport :
                        {
                            s1ap_handle_uplink_nas_transport(enb, &message);
                            break;
                        }
                        default:
                        {
                            d_warn("Not implemented(choice:%d, proc:%d",
                                    message.direction, message.procedureCode);
                            break;
                        }
                    }

                    break;
                }
                case S1AP_PDU_PR_successfulOutcome :
                {
                    switch(message.procedureCode)
                    {
                        default:
                        {
                            d_warn("Not implemented(choice:%d, proc:%d",
                                    message.direction, message.procedureCode);
                            break;
                        }
                    }
                    break;
                }
                case S1AP_PDU_PR_unsuccessfulOutcome :
                default:
                {
                    d_warn("Not implemented(choice:%d, proc:%d",
                        message.direction, message.procedureCode);
                    break;
                }
            }

            s1ap_free_pdu(&message);
            pkbuf_free(recvbuf);
            break;
        }

        default:
        {
            d_error("Unknown event %s", mme_event_get_name(e));
            break;
        }
    }
}

void enb_s1ap_state_exception(enb_s1ap_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(1, e);

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            break;
        }
        case FSM_EXIT_SIG:
        {
            break;
        }
        default:
        {
            d_error("Unknown event %s", mme_event_get_name(e));
            break;
        }
    }
}

