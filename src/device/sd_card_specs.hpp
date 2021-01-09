
/*******************************************************************************
 * A header file to define types and values of the SD Card specification
 ******************************************************************************/

#ifndef STMINISH_DEVICE_SD_CARD_SPECS_HEADER_HPP
#define STMINISH_DEVICE_SD_CARD_SPECS_HEADER_HPP

#include <cstdint>

namespace sdcard
{
/*******************************************************************************
 * COMMAND FORMATS
 ******************************************************************************/

constexpr unsigned cmd0 = 0;

constexpr unsigned cmd2 = 2;

constexpr unsigned cmd3 = 3;

constexpr unsigned acmd6               = 6;
constexpr unsigned acmd6_bus_width_pos = 0;
constexpr unsigned acmd6_bus_width     = 0x3 << acmd6_bus_width_pos;

constexpr unsigned cmd7 = 7;

constexpr unsigned cmd8                    = 8;
constexpr unsigned cmd8_supply_voltage_pos = 8;
constexpr unsigned cmd8_supply_voltage     = 0xF << cmd8_supply_voltage_pos;
constexpr unsigned cmd8_check_pattern_pos  = 0;
constexpr unsigned cmd8_check_pattern      = 0xFF << cmd8_check_pattern_pos;

constexpr unsigned cmd9 = 9;

constexpr unsigned cmd11 = 11;

constexpr unsigned acmd41          = 41;
constexpr unsigned acmd41_hcs_pos  = 30;
constexpr unsigned acmd41_hcs      = 1 << acmd41_hcs_pos;
constexpr unsigned acmd41_xpc_pos  = 28;
constexpr unsigned acmd41_xpc      = 1 << acmd41_xpc_pos;
constexpr unsigned acmd41_s18r_pos = 24;
constexpr unsigned acmd41_s18r     = 1 << acmd41_s18r_pos;
constexpr unsigned acmd41_ocr_pos  = 8;
constexpr unsigned acmd41_ocr      = 0xFFFF << acmd41_ocr_pos;

constexpr unsigned cmd55 = 55;

constexpr unsigned cmd_rca_pos = 16;
constexpr unsigned cmd_rca     = 0xFFFF << cmd_rca_pos;


/*******************************************************************************
 * RESPONSE FORMATS
 ******************************************************************************/

constexpr unsigned status_out_of_range_pos   = 31;
constexpr unsigned status_out_of_range       = 1 << status_out_of_range_pos;
constexpr unsigned status_addr_err_pos       = 30;
constexpr unsigned status_addr_err           = 1 << status_addr_err_pos;
constexpr unsigned status_block_len_err_pos  = 29;
constexpr unsigned status_block_len_err      = 1 << status_block_len_err_pos;
constexpr unsigned status_erase_seq_err_pos  = 28;
constexpr unsigned status_erase_seq_err      = 1 << status_erase_seq_err_pos;
constexpr unsigned status_erase_param_pos    = 27;
constexpr unsigned status_erase_param        = 1 << status_erase_param_pos;
constexpr unsigned status_wp_violation_pos   = 26;
constexpr unsigned status_wp_violation       = 1 << status_wp_violation_pos;
constexpr unsigned status_card_is_locked_pos = 25;
constexpr unsigned status_card_is_locked     = 1 << status_card_is_locked_pos;
constexpr unsigned status_lock_unlock_failed_pos = 24;
constexpr unsigned status_lock_unlock_failed     = 1
                                               << status_lock_unlock_failed_pos;
constexpr unsigned status_com_crc_err_pos     = 23;
constexpr unsigned status_com_crc_err         = 1 << status_com_crc_err_pos;
constexpr unsigned status_illegal_cmd_pos     = 22;
constexpr unsigned status_illegal_cmd         = 1 << status_illegal_cmd_pos;
constexpr unsigned status_card_ecc_failed_pos = 21;
constexpr unsigned status_card_ecc_failed     = 1 << status_card_ecc_failed_pos;
constexpr unsigned status_cc_err_pos          = 20;
constexpr unsigned status_cc_err              = 1 << status_cc_err_pos;
constexpr unsigned status_err_pos             = 19;
constexpr unsigned status_err                 = 1 << status_err_pos;
constexpr unsigned status_csd_overwrite_pos   = 16;
constexpr unsigned status_csd_overwrite       = 1 << status_csd_overwrite_pos;
constexpr unsigned status_wp_erase_skip_pos   = 15;
constexpr unsigned status_wp_erase_skip       = 1 << status_wp_erase_skip_pos;
constexpr unsigned status_card_ecc_disabled_pos = 14;
constexpr unsigned status_card_ecc_disabled = 1 << status_card_ecc_disabled_pos;
constexpr unsigned status_erase_reset_pos   = 13;
constexpr unsigned status_erase_reset       = 1 << status_erase_reset_pos;
constexpr unsigned status_current_state_pos = 9;
constexpr unsigned status_current_state     = 0xF << status_current_state_pos;
constexpr unsigned status_ready_for_data_pos = 8;
constexpr unsigned status_ready_for_data     = 1 << status_ready_for_data_pos;
constexpr unsigned status_fx_event_pos       = 6;
constexpr unsigned status_fx_event           = 1 << status_fx_event_pos;
constexpr unsigned status_app_cmd_pos        = 5;
constexpr unsigned status_app_cmd            = 1 << status_app_cmd_pos;
constexpr unsigned status_ake_seq_err_pos    = 3;
constexpr unsigned status_ake_seq_err        = 1 << status_ake_seq_err_pos;
/* A mask to display all error bits */
constexpr unsigned status_err_msk = 0xF9D90008;

enum class SdCardState { Idle, Ready, Ident, Stby, Tran, Data, Rcv, Prg, Dis };

constexpr unsigned r2_1_csize_pos = 16;
constexpr unsigned r2_1_csize     = 0xFFFF << r2_1_csize_pos;


constexpr unsigned r3_busy_pos = 31;
constexpr unsigned r3_busy     = 1 << r3_busy_pos;
constexpr unsigned r3_ccs_pos  = 30;
constexpr unsigned r3_ccs      = 1 << r3_ccs_pos;
constexpr unsigned r3_uhs2_pos = 29;
constexpr unsigned r3_uhs2     = 1 << r3_uhs2_pos;
constexpr unsigned r3_s18a_pos = 24;
constexpr unsigned r3_s18a     = 1 << r3_s18a_pos;
constexpr unsigned r3_ocr_pos  = 8;
constexpr unsigned r3_ocr      = 0xFFFF << r3_ocr_pos;

constexpr unsigned r6_rca_pos            = 16;
constexpr unsigned r6_rca                = 0xFFFF << r6_rca_pos;
constexpr unsigned r6_com_crc_err_pos    = 15;
constexpr unsigned r6_com_crc_err        = 1 << r6_com_crc_err_pos;
constexpr unsigned r6_illegal_cmd_pos    = 14;
constexpr unsigned r6_illegal_cmd        = 1 << r6_illegal_cmd_pos;
constexpr unsigned r6_err_pos            = 13;
constexpr unsigned r6_err                = 1 << r6_err_pos;
constexpr unsigned r6_current_state_pos  = 9;
constexpr unsigned r6_current_state      = 0xF << r6_current_state_pos;
constexpr unsigned r6_ready_for_data_pos = 8;
constexpr unsigned r6_ready_for_data     = 1 << r6_ready_for_data_pos;
constexpr unsigned r6_fx_event_pos       = 6;
constexpr unsigned r6_fx_event           = 1 << r6_fx_event_pos;
constexpr unsigned r6_app_cmd_pos        = 5;
constexpr unsigned r6_app_cmd            = 1 << r6_app_cmd_pos;
constexpr unsigned r6_ake_seq_err_pos    = 3;
constexpr unsigned r6_ake_seq_err        = 1 << r6_ake_seq_err_pos;
constexpr unsigned r6_err_msk            = 0xE008;


}  // namespace sdcard

#endif
