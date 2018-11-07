#include "SDprint.h"
#include "SDcard.h"
#include "uart.h"

// new comment asdf blah
void SD_printR1(uint8_t res)
{
    if(res & 0b10000000)
        { UART_pputs("\tError: MSB = 1\r\n"); return; }
    if(res == 0)
        { UART_pputs("\tCard Ready\r\n"); return; }
    if(PARAM_ERROR(res))
        UART_pputs("\tParameter Error\r\n");
    if(ADDR_ERROR(res))
        UART_pputs("\tAddress Error\r\n");
    if(ERASE_SEQ_ERROR(res))
        UART_pputs("\tErase Sequence Error\r\n");
    if(CRC_ERROR(res))
        UART_pputs("\tCRC Error\r\n");
    if(ILLEGAL_CMD(res))
        UART_pputs("\tIllegal Command\r\n");
    if(ERASE_RESET(res))
        UART_pputs("\tErase Reset Error\r\n");
    if(IN_IDLE(res))
        UART_pputs("\tIn Idle State\r\n");
}

void SD_printR2(uint8_t *res)
{
    SD_printR1(res[0]);

    if(res[1] == 0x00)
        UART_pputs("\tNo R2 Error\r\n");
    if(OUT_OF_RANGE(res[1]))
        UART_pputs("\tOut of Range\r\n");
    if(ERASE_PARAM(res[1]))
        UART_pputs("\tErase Parameter\r\n");
    if(WP_VIOLATION(res[1]))
        UART_pputs("\tWP Violation\r\n");
    if(CARD_ECC_FAILED(res[1]))
        UART_pputs("\tECC Failed\r\n");
    if(CC_ERROR(res[1]))
        UART_pputs("\tCC Error\r\n");
    if(ERROR(res[1]))
        UART_pputs("\tError\r\n");
    if(WP_ERASE_SKIP(res[1]))
        UART_pputs("\tWP Erase Skip\r\n");
    if(CARD_LOCKED(res[1]))
        UART_pputs("\tCard Locked\r\n");
}

void SD_printR3(uint8_t *res)
{
    SD_printR1(res[0]);

    if(res[0] > 1) return;

    UART_pputs("\tCard Power Up Status: ");
    if(POWER_UP_STATUS(res[1]))
    {
        UART_pputs("READY\r\n");
        UART_pputs("\tCCS Status: ");
        if(CCS_VAL(res[1])){ UART_pputs("1\r\n"); }
        else UART_pputs("0\r\n");
    }
    else
    {
        UART_pputs("BUSY\r\n");
    }

    UART_pputs("\tVDD Window: ");
    if(VDD_2728(res[3])) UART_pputs("2.7-2.8, ");
    if(VDD_2829(res[2])) UART_pputs("2.8-2.9, ");
    if(VDD_2930(res[2])) UART_pputs("2.9-3.0, ");
    if(VDD_3031(res[2])) UART_pputs("3.0-3.1, ");
    if(VDD_3132(res[2])) UART_pputs("3.1-3.2, ");
    if(VDD_3233(res[2])) UART_pputs("3.2-3.3, ");
    if(VDD_3334(res[2])) UART_pputs("3.3-3.4, ");
    if(VDD_3435(res[2])) UART_pputs("3.4-3.5, ");
    if(VDD_3536(res[2])) UART_pputs("3.5-3.6");
    UART_pputs("\r\n");
}

void SD_printR7(uint8_t *res)
{
    SD_printR1(res[0]);

    if(res[0] > 1) return;

    UART_pputs("\tCommand Version: ");
    UART_puthex8(CMD_VER(res[1]));
    UART_pputs("\r\n");

    UART_pputs("\tVoltage Accepted: ");
    if(VOL_ACC(res[3]) == VOLTAGE_ACC_27_33)
        UART_pputs("2.7-3.6V\r\n");
    else if(VOL_ACC(res[3]) == VOLTAGE_ACC_LOW)
        UART_pputs("LOW VOLTAGE\r\n");
    else if(VOL_ACC(res[3]) == VOLTAGE_ACC_RES1)
        UART_pputs("RESERVED\r\n");
    else if(VOL_ACC(res[3]) == VOLTAGE_ACC_RES2)
        UART_pputs("RESERVED\r\n");
    else
        UART_pputs("NOT DEFINED\r\n");

    UART_pputs("\tEcho: ");
    UART_puthex8(res[4]);
    UART_pputs("\r\n");
}

void SD_printCSD(uint8_t *buf)
{
    UART_pputs("CSD:\r\n");

    UART_pputs("\tCSD Structure: ");
    UART_puthex8((buf[0] & 0b11000000) >> 6);
    UART_pputs("\r\n");

    UART_pputs("\tTAAC: ");
    UART_puthex8(buf[1]);
    UART_pputs("\r\n");

    UART_pputs("\tNSAC: ");
    UART_puthex8(buf[2]);
    UART_pputs("\r\n");

    UART_pputs("\tTRAN_SPEED: ");
    UART_puthex8(buf[3]);
    UART_pputs("\r\n");

    UART_pputs("\tDevice Size: ");
    UART_puthex8(buf[7] & 0b00111111);
    UART_puthex8(buf[8]);
    UART_puthex8(buf[9]);
    UART_pputs("\r\n");
}

void SD_printBuf(uint8_t *buf)
{
    for(uint16_t i = 0; i < 512; i++) UART_puthex8(*buf++);
    UART_pputs("\r\n");
}

void SD_printDataResponseToken(uint8_t token)
{
    if(((token) & SD_RESPONSE_TOKEN_MASK) == SD_DATA_ACCEPTED)
        UART_pputs("\tData Accepted\r\n");
    else if(((token) & SD_RESPONSE_TOKEN_MASK) == SD_DATA_REJECTED_CRC)
        UART_pputs("\tData Rejected due to CRC\r\n");
    else if(((token) & SD_RESPONSE_TOKEN_MASK) == SD_DATA_REJECTED_WRITE)
        UART_pputs("\tData Rejected due to write error\r\n");
    else
    {
        UART_pputs("\tError: Response token of bad form: ");
        UART_puthex8(token);
        UART_pputs("\r\n");
    }
}
