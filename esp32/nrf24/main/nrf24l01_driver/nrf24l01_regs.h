#ifndef NRF24L01_REGS_H_
#define NRF24L01_REGS_H_

// Register Map
#define CONFIG 0x00
#define EN_AA 0x01
#define EN_RXADDR 0x02
#define SETUP_AW 0x03
#define SETUP_RETR 0x04
#define RF_CH 0x05
#define RF_SETUP 0x06
#define STATUS 0x07
#define OBSERVE_TX 0x08
#define CD 0x09 // Carrier Detect (nRF24L01 only)
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17
#define DYNPD 0x1C
#define FEATURE 0x1D

// Bit Mnemonics - CONFIG register
#define MASK_RX_DR (1 << 6)
#define MASK_TX_DS (1 << 5)
#define MASK_MAX_RT (1 << 4)
#define EN_CRC (1 << 3)
#define CRCO (1 << 2)
#define PWR_UP (1 << 1)
#define PRIM_RX (1 << 0)

// Bit Mnemonics - STATUS register
#define RX_DR (1 << 6)
#define TX_DS (1 << 5)
#define MAX_RT (1 << 4)

// Bit Mnemonics - RF_SETUP register
#define RF_DR_LOW (1 << 5) // nRF24L01+ only
#define PLL_LOCK (1 << 4)
#define RF_DR_HIGH (1 << 3)
#define PWR (3 << 1) // 00=-18dBm, 01=-12dBm, 10=-6dBm, 11=0dBm

// Commands
#define R_REGISTER 0x00
#define W_REGISTER 0x20
#define R_RX_PAYLOAD 0x61
#define W_TX_PAYLOAD 0xA0
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
#define REUSE_TX_PL 0xE3
#define NOP 0xFF

// W_REGISTER | REGISTER is needed when writing
// R_REGISTER | REGISTER when reading

#endif /* NRF24L01_REGS_H_ */
