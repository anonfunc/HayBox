#include "modes/DefaultKeyboardMode.hpp"

#include "core/socd.hpp"
#include "core/state.hpp"

DefaultKeyboardMode::DefaultKeyboardMode(socd::SocdType socd_type) : KeyboardMode(socd_type) {}

void DefaultKeyboardMode::UpdateKeys(InputState &inputs) {
    Press(HID_KEY_SHIFT_LEFT, inputs.l);
    Press(HID_KEY_A, inputs.left);
    Press(HID_KEY_S, inputs.down);
    Press(HID_KEY_D, inputs.right);
    Press(HID_KEY_W, inputs.mod_x);
    Press(HID_KEY_SPACE, inputs.mod_y);
    Press(HID_KEY_E, inputs.nunchuk_c);

    Press(HID_KEY_G, inputs.select);
    Press(HID_KEY_ESCAPE, inputs.start);
    Press(HID_KEY_I, inputs.home);

    Press(HID_KEY_U, inputs.r);
    Press(HID_KEY_I, inputs.y);
    Press(HID_KEY_O, inputs.lightshield);
    Press(HID_KEY_P, inputs.midshield);

    Press(HID_KEY_X, inputs.b);
    Press(HID_KEY_C, inputs.x);
    Press(HID_KEY_V, inputs.z);
    Press(HID_KEY_Q, inputs.up);

    Press(HID_KEY_ARROW_UP, inputs.c_up);
    Press(HID_KEY_ARROW_LEFT, inputs.c_left);
    Press(HID_KEY_ARROW_RIGHT, inputs.c_right);
    Press(HID_KEY_Z, inputs.a);
    Press(HID_KEY_ARROW_DOWN, inputs.c_down);

}
