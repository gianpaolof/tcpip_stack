/*
 * =====================================================================================
 *
 *       Filename:  netfilter.h
 *
 *    Description: This file defines the interfaces to work with Netfilter Hooks 
 *
 *        Version:  1.0
 *        Created:  02/13/2021 02:45:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ABHISHEK SAGAR (), sachinites@gmail.com
 *   Organization:  Juniper Networks
 *
 * =====================================================================================
 */

#ifndef __NF__
#define __NF__

#include "../notif.h"

typedef enum {

	NF_IP_PRE_ROUTING,
	NF_IP_LOCAL_IN,
	NF_IP_FORWARD,
	NF_IP_LOCAL_OUT,
	NF_IP_POST_ROUTING,
	NF_IP_END
} nf_hook_t;

typedef struct nf_hook_db_ {

	notif_chain_t nf_hook[NF_IP_END];
} nf_hook_db_t;

void
nf_init_netfilters();

typedef struct node_ node_t;
typedef struct interface_ interface_t;

void
nf_invoke_netfilter_hook(nf_hook_t nf_hook_type,
                         char *pkt,
                         size_t pkt_size,
                         node_t *node,
                         interface_t *intf);
void
nf_register_netfilter_hook(nf_hook_t nf_hook_type,
                           nfc_pkt_trap pkt_trap_cb,
                           nfc_app_cb pkt_notif_app_cb);

#endif 
