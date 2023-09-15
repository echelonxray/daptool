#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

signed int oper_write_reg(DAP_Connection* dap_con, unsigned int reg, uint32_t value) {
	unsigned int tr_req;
	tr_req = (reg & 0xC) | DAP_TRANSFER_MODE_WRITE;
	if (reg & OPER_REG_ACCESSMASK) {
		if (dap_con->sel_addr != (reg & 0xF0)) {
			oper_write_reg(dap_con, OPER_REG_DEBUG_SELECT, reg & 0xF0); // TODO: Handle Return Value
		}
		tr_req |= DAP_TRANSFER_ACCESS_PORT;
	}

	// Transfer
	{
		signed int retval;
		unsigned char transfer_request[] = {
			tr_req,
		};
		uint32_t transfer_buffer[] = {
			value,
		};
		retval = dap_transfer(dap_con,
		                      0,
		                      sizeof(transfer_request) / sizeof(*transfer_request),
		                      transfer_request,
		                      transfer_buffer);
		assert(retval == 0); // TODO: Replace with return error code
		if (tr_req == (DAP_TRANSFER_DEBUG_PORT | DAP_TRANSFER_MODE_WRITE | OPER_REG_DEBUG_SELECT)) {
			dap_con->sel_addr = value & 0xF0;
		}
	}

	return 0;
}
signed int oper_read_reg(DAP_Connection* dap_con, unsigned int reg, uint32_t* buffer) {
	unsigned int tr_req;
	tr_req = (reg & 0xC) | DAP_TRANSFER_MODE_READ;
	if (reg & OPER_REG_ACCESSMASK) {
		if (dap_con->sel_addr != (reg & 0xF0)) {
			oper_write_reg(dap_con, OPER_REG_DEBUG_SELECT, reg & 0xF0); // TODO: Handle Return Value
		}
		tr_req |= DAP_TRANSFER_ACCESS_PORT;
	}

	// Transfer
	{
		signed int retval;
		unsigned char transfer_request[] = {
			tr_req,
		};
		uint32_t transfer_buffer[] = {
			0x00000000,
		};
		retval = dap_transfer(dap_con,
		                      0,
		                      sizeof(transfer_request) / sizeof(*transfer_request),
		                      transfer_request,
		                      transfer_buffer);
		assert(retval == 0); // TODO: Replace with return error code
		*buffer = transfer_buffer[0];
	}

	return 0;
}

signed int oper_write_mem8(DAP_Connection* dap_con, uint32_t address, uint8_t value) {
	// Transfer - Set 1 byte address mode.  Disable auto-increment mode.
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_CSW, OPER_REG_ACCESS_CSWMODE_08BITADDR |
		                                                      OPER_REG_ACCESS_CSWMODE_NOAUTOINC);
		assert(retval == 0);
	}
	// Transfer - Set address
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_TAR, address);
		assert(retval == 0);
	}
	// Transfer - Write to address
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_DRW, value);
		assert(retval == 0);
	}

	return 0;
}
signed int oper_read_mem8(DAP_Connection* dap_con, uint32_t address, uint8_t* buffer) {
	// Transfer - Set 1 byte address mode.  Disable auto-increment mode.
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_CSW, OPER_REG_ACCESS_CSWMODE_08BITADDR |
		                                                      OPER_REG_ACCESS_CSWMODE_NOAUTOINC);
		assert(retval == 0);
	}
	// Transfer - Set address
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_TAR, address);
		assert(retval == 0);
	}
	// Transfer - Read from address
	{
		signed int retval;
		uint32_t tmp;
		retval = oper_read_reg(dap_con, OPER_REG_ACCESS_DRW, &tmp);
		assert(retval == 0);
		*buffer = tmp;
	}

	return 0;
}

signed int oper_write_mem16(DAP_Connection* dap_con, uint32_t address, uint16_t value) {
	if (address & 0x1) {
		printf("Error: oper_write_mem32(): Address Misaligned: 0x%08X.\n", address);
		return -5;
	}

	// Transfer - Set 2 byte address mode.  Disable auto-increment mode.
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_CSW, OPER_REG_ACCESS_CSWMODE_16BITADDR |
		                                                      OPER_REG_ACCESS_CSWMODE_NOAUTOINC);
		assert(retval == 0);
	}
	// Transfer - Set address
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_TAR, address);
		assert(retval == 0);
	}
	// Transfer - Write to address
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_DRW, value);
		assert(retval == 0);
	}

	return 0;
}
signed int oper_read_mem16(DAP_Connection* dap_con, uint32_t address, uint16_t* buffer) {
	if (address & 0x1) {
		printf("Error: oper_read_mem32(): Address Misaligned: 0x%08X.\n", address);
		return -5;
	}

	// Transfer - Set 2 byte address mode.  Disable auto-increment mode.
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_CSW, OPER_REG_ACCESS_CSWMODE_16BITADDR |
		                                                      OPER_REG_ACCESS_CSWMODE_NOAUTOINC);
		assert(retval == 0);
	}
	// Transfer - Set address
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_TAR, address);
		assert(retval == 0);
	}
	// Transfer - Read from address
	{
		signed int retval;
		uint32_t tmp;
		retval = oper_read_reg(dap_con, OPER_REG_ACCESS_DRW, &tmp);
		assert(retval == 0);
		*buffer = tmp;
	}

	return 0;
}

signed int oper_write_mem32(DAP_Connection* dap_con, uint32_t address, uint32_t value) {
	if (address & 0x3) {
		printf("Error: oper_write_mem32(): Address Misaligned: 0x%08X.\n", address);
		return -5;
	}

	// Transfer - Set 4 byte address mode.  Disable auto-increment mode.
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_CSW, OPER_REG_ACCESS_CSWMODE_32BITADDR |
		                                                      OPER_REG_ACCESS_CSWMODE_NOAUTOINC);
		assert(retval == 0);
	}
	// Transfer - Set address
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_TAR, address);
		assert(retval == 0);
	}
	// Transfer - Write to address
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_DRW, value);
		assert(retval == 0);
	}

	return 0;
}
signed int oper_read_mem32(DAP_Connection* dap_con, uint32_t address, uint32_t* buffer) {
	if (address & 0x3) {
		printf("Error: oper_read_mem32(): Address Misaligned: 0x%08X.\n", address);
		return -5;
	}

	// Transfer - Set 4 byte address mode.  Disable auto-increment mode.
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_CSW, OPER_REG_ACCESS_CSWMODE_32BITADDR |
		                                                      OPER_REG_ACCESS_CSWMODE_NOAUTOINC);
		assert(retval == 0);
	}
	// Transfer - Set address
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_TAR, address);
		assert(retval == 0);
	}
	// Transfer - Read from address
	{
		signed int retval;
		uint32_t tmp;
		retval = oper_read_reg(dap_con, OPER_REG_ACCESS_DRW, &tmp);
		assert(retval == 0);
		*buffer = tmp;
	}

	return 0;
}

signed int oper_write_memblock32(DAP_Connection* dap_con, uint32_t address, uint32_t* values, uint32_t buffer_length) {
	if (address & 0x3) {
		printf("Error: oper_write_memblock32(): Address Misaligned: 0x%08X.\n", address);
		return -5;
	}

	// Transfer - Set 4 byte address mode.  Enable auto-increment mode.
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_CSW, OPER_REG_ACCESS_CSWMODE_32BITADDR |
		                                                      OPER_REG_ACCESS_CSWMODE_AUTOINC);
		assert(retval == 0);
	}
	// Transfer - Set address
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_TAR, address);
		assert(retval == 0);
	}
	// Transfer - Write to address
	for (uint32_t i = 0; i < buffer_length; i++) {
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_DRW, values[i]);
		assert(retval == 0);
	}

	return 0;
}
signed int oper_read_memblock32(DAP_Connection* dap_con, uint32_t address, uint32_t* buffer, uint32_t buffer_length) {
	if (address & 0x3) {
		printf("Error: oper_read_memblock32(): Address Misaligned: 0x%08X.\n", address);
		return -5;
	}

	// Transfer - Set 4 byte address mode.  Enable auto-increment mode.
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_CSW, OPER_REG_ACCESS_CSWMODE_32BITADDR |
		                                                      OPER_REG_ACCESS_CSWMODE_AUTOINC);
		assert(retval == 0);
	}
	// Transfer - Set address
	{
		signed int retval;
		retval = oper_write_reg(dap_con, OPER_REG_ACCESS_TAR, address);
		assert(retval == 0);
	}
	// Transfer - Read from address
	for (uint32_t i = 0; i < buffer_length; i++) {
		signed int retval;
		uint32_t tmp;
		retval = oper_read_reg(dap_con, OPER_REG_ACCESS_DRW, &tmp);
		assert(retval == 0);
		buffer[i] = tmp;
	}

	return 0;
}

/*
signed int oper_program_flash(DAP_Connection* dap_con, uint32_t address, uint8_t* buffer, uint32_t buffer_length) {
	if (address & 0x3) {
		printf("Error: oper_read_memblock32(): Address Misaligned: 0x%08X.\n", address);
		return -5;
	}

	uint32_t size_of_used_pages;
	uint32_t start_page_address;
	uint32_t end_page_address;
	start_page_address = address & 0x;
	size_of_used_pages
	void* page_backup;
	page_backup = malloc(size_of_used_pages);
	assert(! oper_read_memblock32(dap_con, address, page_backup, size_of_used_pages / 4) );

	return 0;
}
*/

signed int oper_init(DAP_Connection** dap_con, libusb_device_handle* d_handle) {
	DAP_Connection* local_dap_con;

	local_dap_con = malloc(sizeof(DAP_Connection));
	local_dap_con->device_handle = d_handle;

	// Connect
	{
		signed int retval;
		retval = dap_connect(local_dap_con, DAP_CONNECT_PORT_MODE_SWD);
		assert(retval == 0);
		printf("Connect.\n");
	}

	// SWJ Sequence
	{
		signed int retval;
		unsigned char sequence[] = {
			0x00,
		};
		retval = dap_swj_sequence(local_dap_con, 0x08, sequence, sizeof(sequence) / sizeof(*sequence));
		assert(retval == 0);
		printf("SWJ Sequence.\n");
	}

	// Transfer - Read ID register
	{
		signed int retval;
		uint32_t buffer;
		retval = oper_read_reg(local_dap_con, OPER_REG_DEBUG_IDCODE, &buffer);
		assert(retval == 0);
		printf("Transfer - Read Debug Port IDCODE register: 0x%08X.\n", buffer);
	}

	// Transfer - Enable AP register access and configure DB SELECT to enable access to the AP ID register
	{
		signed int retval;
		retval = oper_write_reg(local_dap_con, OPER_REG_DEBUG_CTRLSTAT, 0x50000000);
		assert(retval == 0);
		retval = oper_write_reg(local_dap_con, OPER_REG_DEBUG_SELECT, 0x000000F0);
		assert(retval == 0);
		printf("Transfer - Enable AHB-AP register access and set the SELECT register.\n");
	}

	// Transfer - Read ID register
	{
		signed int retval;
		uint32_t buffer;
		retval = oper_read_reg(local_dap_con, OPER_REG_ACCESS_IDR, &buffer);
		assert(retval == 0);
		printf("Transfer - Read Access Port IDR register: 0x%08X.\n", buffer);
	}

	*dap_con = local_dap_con;

	return 0;
}
signed int oper_destroy(DAP_Connection* dap_con) {
	// Disconnect
	{
		signed int retval;
		retval = dap_disconnect(dap_con);
		assert(retval == 0);
		printf("Disconnect.\n");
	}

	free(dap_con);

	return 0;
}
