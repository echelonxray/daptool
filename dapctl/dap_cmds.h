// SPDX-License-Identifier: 0BSD
/*
 * Copyright (C) 2023 Michael T. Kloos <michael@michaelkloos.com>
 */

#ifndef _HEADERINC_DAPCTL_CMD_H
#define _HEADERINC_DAPCTL_CMD_H

#include "../main.h"
#include "../dapctl.h"
#include <stdint.h>

#define DAP_STATUS_OK    0x00
#define DAP_STATUS_ERROR 0xFF

#define DAP_INFO 0x00
#define DAP_INFO_ID_VENDOR_NAME               0x01
#define DAP_INFO_ID_PRODUCT_NAME              0x02
#define DAP_INFO_ID_SERIAL_NUMBER             0x03
#define DAP_INFO_ID_CMSISDAP_PROTOCOL_VERSION 0x04
#define DAP_INFO_ID_TARGET_DEVICE_VENDOR      0x05
#define DAP_INFO_ID_TARGET_DEVICE_NAME        0x06
#define DAP_INFO_ID_TARGET_BOARD_VENDOR       0x07
#define DAP_INFO_ID_TARGET_BOARD_NAME         0x08
#define DAP_INFO_ID_PRODUCT_FIRMWARE_VERSION  0x09
#define DAP_INFO_ID_CAPABILITIES              0xF0
#define DAP_INFO_ID_TEST_DOMAIN_TIMER         0xF1
#define DAP_INFO_ID_UART_RECEIVE_BUFFER_SIZE  0xFB
#define DAP_INFO_ID_UART_TRANSMIT_BUFFER_SIZE 0xFC
#define DAP_INFO_ID_SWO_TRACE_BUFFER_SIZE     0xFD
#define DAP_INFO_ID_PACKET_COUNT              0xFE
#define DAP_INFO_ID_PACKET_SIZE               0xFF

#define DAP_CONNECT 0x02
#define DAP_CONNECT_PORT_MODE_DEFAULT 0x00
#define DAP_CONNECT_PORT_MODE_SWD     0x01
#define DAP_CONNECT_PORT_MODE_JTAG    0x02

#define DAP_DISCONNECT 0x03

#define DAP_SWJ_CLOCK 0x11
#define DAP_SWJ_SEQUENCE 0x12

#define DAP_TRANSFER 0x05
#define DAP_TRANSFER_DEBUG_PORT  0x0
#define DAP_TRANSFER_ACCESS_PORT 0x1
#define DAP_TRANSFER_MODE_WRITE  0x0
#define DAP_TRANSFER_MODE_READ   0x2

signed int dap_connect(DAP_Connection* dap_con, unsigned int mode);
signed int dap_disconnect(DAP_Connection* dap_con);

signed int dap_info(DAP_Connection* dap_con, unsigned int id);

signed int dap_transfer(DAP_Connection* dap_con, unsigned int dap_index, unsigned int transfer_count, unsigned char* req_buffer, uint32_t* data_buffer);

signed int dap_swj_clock(DAP_Connection* dap_con, uint32_t clock_setting);
signed int dap_swj_sequence(DAP_Connection* dap_con, unsigned int bit_count, unsigned char* data_buffer, unsigned int data_buffer_length);

#endif
