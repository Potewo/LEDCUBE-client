#include <Arduino.h>
#include <PacketSerial.h>
#include <SPT.h>

#define SER 8
#define CLK 9
#define RCLK 10

SPT spt;
PacketSerial ps;

uint8_t d[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t data[8][8] = {
  {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}, 
  {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}, 
  {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}, 
  {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}, 
  {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}, 
  {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}, 
  {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}, 
  {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}
};


uint8_t data2[8][8] = {
  {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}, 
  {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}, 
  {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}, 
  {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}, 
  {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}, 
  {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}, 
  {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}, 
  {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}
};

const uint8_t cathods[] = {
  11,
  12,
  13,
  3,
  4,
  5,
  6,
  7
};

const int cathodsn = sizeof(cathods) / sizeof(cathods[0]);

void onPacketReceived(const uint8_t* buffer, size_t size);

uint8_t buf[64];
size_t buf_size = 8;

void setup() {
  delay(1000);
  spt.init(SER, CLK, RCLK);
  spt.setup();
  ps.begin(115200);
  ps.setPacketHandler(&onPacketReceived);
  for (int i = 0; i < cathodsn; i++) {
    pinMode(cathods[i], OUTPUT);
  }
  for (int i = 0; i < 64; i++) {
    spt.send(d, 8);
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      buf[8*i+j] = data[i][j];
    }
  }
}

void loop() {
  ps.update();
  if (ps.overflow()) {
  }
  for (int i = 0; i < cathodsn; i++) {
    spt.send(buf + i*8, buf_size);
    digitalWrite(cathods[i], HIGH);
    delay(1);
    digitalWrite(cathods[i], LOW);
  }
}

void onPacketReceived(const uint8_t* buffer, size_t size) {
  memcpy(buf, buffer, size);
}
