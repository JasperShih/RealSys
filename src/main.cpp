// //Author: 	Tso-Hsien Shih

#include <mbed.h>
#include <math.h>
#include <USBSerial.h>
#include <vector>
#include <chrono>
#include "LIS3DSH.h"

class AnglePair {
public:
    int roll;
    int pitch;
};

enum Action {
    original,
    situp_lying,
    stand,
    squat_down,
    pushup_down,
    jump_horizontal1,
    jump_vertical,
    jump_horizontal2,
};

class GeneralAction {
public:
    AnglePair stand      = AnglePair{105, 165};
    AnglePair bias_stand = AnglePair{25, 20};
};

//original->lying->sit->original
class SitUp {
public:
    AnglePair lying      = AnglePair{320, 292};
    AnglePair bias_lying = AnglePair{40, 40};
    AnglePair sit        = AnglePair{205, 320};
    AnglePair bias_sit   = AnglePair{15, 15};
};

//original->stand->squat_down->stand->original
class Squat {
public:
    AnglePair stand      = GeneralAction().stand;
    AnglePair bias_stand = GeneralAction().bias_stand;
    AnglePair down       = AnglePair{170, 110};
    AnglePair bias_down  = AnglePair{10, 35};
};

//original->stand->pushup_down->stand->original
class PushUp {
public:
    AnglePair stand      = GeneralAction().stand;
    AnglePair bias_stand = GeneralAction().bias_stand;
    AnglePair down       = AnglePair{50, 110};
    AnglePair bias_down  = AnglePair{20, 20};
};

//original->stand->horizontal_1->jump_vertical->horizontal_2->stand->original
class JumpingJack {
public:
    AnglePair stand          = GeneralAction().stand;
    AnglePair bias_stand     = GeneralAction().bias_stand;
    AnglePair horizontal     = AnglePair{175, 60};
    AnglePair bias_horizotal = AnglePair{15, 10};
    AnglePair vertical       = AnglePair{180, 30};
    AnglePair bias_vertical  = AnglePair{10, 15};
};

using namespace std;
#define isize(var) (int)var.size()

/* USBSerial library for serial terminal */
USBSerial serial(0x1f00, 0x2012, 0x0001, false);

/* LIS3DSH Library for accelerometer  - using SPI*/
LIS3DSH acc(PA_7, SPI_MISO, SPI_SCK, PE_3);

DigitalOut led_situp(LED1);
DigitalOut led_pushup(LED2);
DigitalOut led_squat(LED5);
DigitalOut led_jump(LED6);
DigitalIn  btn_blue(USER_BUTTON);

int cnt_target = 5;
int cnt_situp  = 0;
int cnt_pushup = 0;
int cnt_squat  = 0;
int cnt_jump   = 0;

GeneralAction                    general_act    = GeneralAction();
SitUp                            situp          = SitUp();
Squat                            squat          = Squat();
PushUp                           pushup         = PushUp();
JumpingJack                      jumpingjack    = JumpingJack();
Action                           state          = original;
chrono::steady_clock::time_point action_updated = chrono::steady_clock::now();

void trans_state(int idx_vec, vector<AnglePair> &vec) {
    int avg2_size   = 2;
    int avg2_roll   = 0;
    int avg2_pitch  = 0;
    int avg20_size  = 20;
    int avg20_roll  = 0;
    int avg20_pitch = 0;

    for (int i = 0; i < avg2_size; ++i) {
        avg2_roll += vec[(isize(vec) + idx_vec - i) % isize(vec)].roll;
        avg2_pitch += vec[(isize(vec) + idx_vec - i) % isize(vec)].pitch;
    }
    avg20_roll += avg2_roll;
    avg20_pitch += avg20_pitch;
    for (int i = avg2_size; i < avg20_size; ++i) {
        avg20_roll += vec[(isize(vec) + idx_vec - i) % isize(vec)].roll;
        avg20_pitch += vec[(isize(vec) + idx_vec - i) % isize(vec)].pitch;
    }
    avg2_roll /= avg2_size;
    avg2_pitch /= avg2_size;
    avg20_roll /= avg20_size;
    avg20_pitch /= avg20_size;

    if (chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - action_updated).count() > 15) {
        state          = original;
        action_updated = chrono::steady_clock::now();
        serial.printf("set state to original because there was no action updates for 10 secs\r\n");
    }

    switch (state) {
        case original:
            if (abs(vec[idx_vec].roll - general_act.stand.roll) < general_act.bias_stand.roll &&
                abs(vec[idx_vec].pitch - general_act.stand.pitch) < general_act.bias_stand.pitch) {
                state          = stand;
                action_updated = chrono::steady_clock::now();
                serial.printf("%d, %d ", avg2_roll, avg2_pitch);
                serial.printf("; stand \r\n");
            } else if (abs(avg2_roll - situp.lying.roll) < situp.bias_lying.roll &&
                       abs(avg2_pitch - situp.lying.pitch) < situp.bias_lying.pitch) {
                state          = situp_lying;
                action_updated = chrono::steady_clock::now();
                serial.printf("%d, %d ", avg2_roll, avg2_pitch);
                serial.printf("; situp_lying \r\n");
            }
            break;
        case situp_lying:
            if (abs(avg2_roll - situp.sit.roll) < situp.bias_sit.roll &&
                abs(avg2_pitch - situp.sit.pitch) < situp.bias_sit.pitch) {
                state          = original;
                action_updated = chrono::steady_clock::now();
                if (cnt_situp + 1 == cnt_target) {
                    led_situp = 0;
                    cnt_situp += 1;
                } else if (cnt_situp < cnt_target) {
                    led_situp = 0;
                    wait_ms(50);
                    led_situp = 1;
                    cnt_situp += 1;
                }
                serial.printf("%d, %d ", avg2_roll, avg2_pitch);
                serial.printf("; situp_sit \r\n");
            }
            break;
        case stand:
            if (abs(avg20_roll - squat.down.roll) < squat.bias_down.roll &&
                abs(avg2_roll - squat.down.roll) < squat.bias_down.roll &&
                abs(avg20_pitch - squat.down.pitch) < squat.bias_down.pitch &&
                abs(avg2_pitch - squat.down.pitch) < squat.bias_down.pitch) {
                state          = squat_down;
                action_updated = chrono::steady_clock::now();
                serial.printf("%d, %d ", avg20_roll, avg20_pitch);
                serial.printf("; squat_down \r\n");
            } else if (abs(vec[idx_vec].roll - jumpingjack.horizontal.roll) < jumpingjack.bias_horizotal.roll &&
                       avg2_pitch <= 65) {
                state          = jump_horizontal1;
                action_updated = chrono::steady_clock::now();
                serial.printf("%d, %d ", vec[idx_vec].roll, vec[idx_vec].pitch);
                serial.printf("; jump_horizontal1 \r\n");
            } else if (abs(avg2_roll - pushup.down.roll) < pushup.bias_down.roll &&
                       abs(avg2_pitch - pushup.down.pitch) < pushup.bias_down.pitch) {
                state          = pushup_down;
                action_updated = chrono::steady_clock::now();
                serial.printf("%d, %d ", avg2_roll, avg2_pitch);
                serial.printf("; pushup_down \r\n");
            }
            break;
        case squat_down:
            if (abs(avg2_roll - general_act.stand.roll) < general_act.bias_stand.roll &&
                abs(avg2_pitch - general_act.stand.pitch) < general_act.bias_stand.pitch) {
                state          = original;
                action_updated = chrono::steady_clock::now();
                // If we finished one time of a sport, LED will blink to give us feedback
                if (cnt_squat + 1 == cnt_target) {
                    led_squat = 0;
                    cnt_squat += 1;
                } else if (cnt_squat < cnt_target) {
                    led_squat = 0;
                    wait_ms(50);
                    led_squat = 1;
                    cnt_squat += 1;
                }
                serial.printf("%d, %d ", avg2_roll, avg2_pitch);
                serial.printf("; original \r\n");
            }
            break;
        case pushup_down:
            if (abs(avg2_roll - general_act.stand.roll) < general_act.bias_stand.roll &&
                abs(avg2_pitch - general_act.stand.pitch) < general_act.bias_stand.pitch) {
                state          = original;
                action_updated = chrono::steady_clock::now();
                if (cnt_pushup + 1 == cnt_target) {
                    led_pushup = 0;
                    cnt_pushup += 1;
                } else if (cnt_pushup < cnt_target) {
                    led_pushup = 0;
                    wait_ms(50);
                    led_pushup = 1;
                    cnt_pushup += 1;
                }
                serial.printf("%d, %d ", avg2_roll, avg2_pitch);
                serial.printf("; original \r\n");
            }
            break;
        case jump_horizontal1:
            if (abs(vec[idx_vec].roll - jumpingjack.vertical.roll) < jumpingjack.bias_vertical.roll &&
                abs(((vec[idx_vec].pitch + 30) % 360) - jumpingjack.vertical.pitch) < jumpingjack.bias_vertical.pitch) {
                state          = jump_vertical;
                action_updated = chrono::steady_clock::now();
                serial.printf("%d, %d ", avg2_roll, avg2_pitch);
                serial.printf("; jump_vertical \r\n");
            }
            break;
        case jump_vertical:
            if (abs(vec[idx_vec].roll - jumpingjack.horizontal.roll) < jumpingjack.bias_horizotal.roll &&
                avg2_pitch <= 65) {
                state          = jump_horizontal2;
                action_updated = chrono::steady_clock::now();
                serial.printf("%d, %d ", avg2_roll, avg2_pitch);
                serial.printf("; jump_horizontal2 \r\n");
            }
            break;
        case jump_horizontal2:
            if (abs(vec[idx_vec].roll - general_act.stand.roll) < general_act.bias_stand.roll &&
                abs(vec[idx_vec].pitch - general_act.stand.pitch) < general_act.bias_stand.pitch) {
                state          = original;
                action_updated = chrono::steady_clock::now();
                if (cnt_jump + 1 == cnt_target) {
                    led_jump = 0;
                    cnt_jump += 1;
                } else if (cnt_jump < cnt_target) {
                    led_jump = 0;
                    wait_ms(50);
                    led_jump = 1;
                    cnt_jump += 1;
                }
                serial.printf("%d, %d ", avg2_roll, avg2_pitch);
                serial.printf("; original \r\n");
            }
            break;
    }
}

int main() {
    int               vlen    = 20;
    int               idx_vec = 0;
    vector<AnglePair> vec(vlen, AnglePair());
    float             roll, pitch;
    bool              used    = false;
    led_situp  = 1;
    led_pushup = 1;
    led_squat  = 1;
    led_jump   = 1;

    for (;;) {
        if (btn_blue == 1 && !used) {
            used       = true;
            cnt_target += 5;
            led_situp  = cnt_situp < cnt_target;
            led_pushup = cnt_pushup < cnt_target;
            led_squat  = cnt_squat < cnt_target;
            led_jump   = cnt_jump < cnt_target;
            wait_ms(100);
            used = false;
        }
        acc.ReadAngles(&roll, &pitch);
        vec[idx_vec].roll  = roll;
        vec[idx_vec].pitch = pitch;

        serial.printf("%d, %d \r\n", (int) vec[idx_vec].roll, (int) vec[idx_vec].pitch);
        trans_state(idx_vec, vec);
        idx_vec = (idx_vec + 1) % vlen;
        wait_ms(50);
    }

    return 0;
}