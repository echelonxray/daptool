// SPDX-License-Identifier: 0BSD
/*
 * Copyright (C) 2023 Michael T. Kloos <michael@michaelkloos.com>
 */

#ifndef _HEADERINC_DAPCTL_H
#define _HEADERINC_DAPCTL_H

#include "main.h"
#include <stdint.h>

typedef struct {
    void* chips_erase_flash_page;
    void* chips_write_to_flash_page;
    void* chips_write_to_flash;
    void* chips_reset;
    void* chips_conn_init;
    void* chips_conn_destroy;
} ChipsPFNs;

typedef struct {
	void* device_handle;
	ChipsPFNs chip_pfns;
	unsigned int sel_addr;
	const char* usbvid_str;
	const char* usbpid_str;
	uint16_t usbvid;
	uint16_t usbpid;
} DAP_Connection;

#include "dapctl/dap_link.h"
#include "dapctl/dap_cmds.h"
#include "dapctl/dap_oper.h"

#endif
