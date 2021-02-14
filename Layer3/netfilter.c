/*
 * =====================================================================================
 *
 *       Filename:  netfilter.c
 *
 *    Description: This file implements the APIs to be used with Netfilter Hooks 
 *
 *        Version:  1.0
 *        Created:  02/13/2021 02:51:40 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ABHISHEK SAGAR (), sachinites@gmail.com
 *   Organization:  Juniper Networks
 *
 * =====================================================================================
 */

#include <string.h>
#include "netfilter.h"
#include "../Layer5/layer5.h"

static nf_hook_db_t nf_hook_db;

static inline void
nf_init_nf_hook(notif_chain_t *nfc,
			  char *nf_hook_name) {

	strcpy(nfc->nfc_name, nf_hook_name);
	init_glthread(&nfc->notif_chain_head);
}
	
void
nf_init_netfilters() {

	nf_init_nf_hook(&nf_hook_db.nf_hook[NF_IP_PRE_ROUTING],
					"NF_IP_PRE_ROUTING");
	nf_init_nf_hook(&nf_hook_db.nf_hook[NF_IP_LOCAL_IN],
					"NF_IP_LOCAL_IN");
	nf_init_nf_hook(&nf_hook_db.nf_hook[NF_IP_FORWARD],
					"NF_IP_FORWARD");
	nf_init_nf_hook(&nf_hook_db.nf_hook[NF_IP_LOCAL_OUT],
					"NF_IP_LOCAL_OUT");
	nf_init_nf_hook(&nf_hook_db.nf_hook[NF_IP_POST_ROUTING],
					"NF_IP_POST_ROUTING");
}

void
nf_invoke_netfilter_hook(nf_hook_t nf_hook_type,
						 char *pkt,
						 size_t pkt_size,
						 node_t *node,
						 interface_t *intf) {

	notif_chain_t *nfc;
	pkt_notif_data_t pkt_notif_data;

	nfc = &nf_hook_db.nf_hook[nf_hook_type];

    pkt_notif_data.recv_node = node;
    pkt_notif_data.recv_interface = intf;
    pkt_notif_data.pkt = pkt;
    pkt_notif_data.pkt_size = pkt_size;

    nfc_invoke_notif_chain(nfc,
			(void *)&pkt_notif_data,
            sizeof(pkt_notif_data_t),
            pkt, pkt_size);
}

void
nf_register_netfilter_hook(nf_hook_t nf_hook_type,
						   nfc_pkt_trap pkt_trap_cb,
						   nfc_app_cb pkt_notif_app_cb) {

	notif_chain_t *nfc;
	notif_chain_elem_t nfce;

	nfc = &nf_hook_db.nf_hook[nf_hook_type];
	
	memset(&nfce, 0, sizeof(notif_chain_elem_t));
	nfce.is_key_set = false;
	nfce.app_cb = pkt_notif_app_cb;
	nfce.pkt_trap_cb = pkt_trap_cb;
	
	nfc_register_notif_chain(nfc, &nfce);
}

