#include "CAN.h"
#include "setting.h"
#include <Arduino.h>
#include "CAN_config.h"

CAN_device_t CAN_cfg;

void setup()
{
    CAN_cfg.tx_pin_id = GPIO_NUM_5;
    CAN_cfg.rx_pin_id = GPIO_NUM_35;
    CAN_cfg.speed = CAN_SPEED_1000KBPS;

    CAN_init();
    Serial.begin(Setting::UARTX::baudRate);
}
void loop()
{
    CAN_frame_t frame{0};
    frame.FIR.B.FF = CAN_frame_std;
    frame.FIR.B.RTR = CAN_no_RTR; // Change the frame to a data frame
    frame.FIR.B.DLC = Setting::Signal::BUFSIZE;

    if (Setting::Signal::BUFSIZE == Serial.readBytes(frame.data.u8, Setting::Signal::BUFSIZE))
    {
        CAN_write_frame(&frame);
    }
}
