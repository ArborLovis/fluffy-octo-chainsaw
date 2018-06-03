

#ifndef DL_RF_MODULE_
#define DL_RF_MODULE_

#define CMD_LEN             3

#define STX_BYTE            0x02
#define CMD_SET_NODE        0x10
#define CMD_GET_NODE        0x11
#define CMD_SET_CHANNEL     0x14
#define CMD_GET_CHANNEL     0x13
#define CMD_GET_FIRMWARE    0x12
#define CMD_SEND_DATA       0x20
#define CMD_DUMMY           0x00

#define CHANNEL_ADR             0x08
#define CAR_NODE_ADR            0x07
#define PC_NODE_ADR             0x09

typedef enum {CHANNEL, CAR_ADDRESS, FIRMWARE} RF_info;

void dlSetChannelRF(uint8_t adress);
void dlSendDataRF(uint8_t rx_adress, uint8_t* data, uint8_t len);
void dlSetCarAddress(uint8_t car_address);

short dlGetSettingInfoRF(RF_info select);
short dlGetDataPayloadRF(uint8_t *payload, uint8_t *len);

#endif //DL_RF_MODULE_
