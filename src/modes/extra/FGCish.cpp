#include "modes/extra/FGCish.hpp"

#define ANALOG_STICK_MIN 0
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 255

FGCish::FGCish(socd::SocdType socd_type) : ControllerMode(socd_type) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,    &InputState::right  },
        socd::SocdPair{ &InputState::down,   &InputState::mod_x  },
        socd::SocdPair{ &InputState::c_left, &InputState::c_right},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up   },
    };

    fgc_left = true;
    toggle_released = true;
}

void FGCish::HandleSocd(InputState &inputs) {
    if (fgc_left) {
        // Manual 
        if (inputs.down && inputs.mod_x) {
            inputs.down = false;
        }
        if (inputs.left && inputs.right) {
            inputs.left = false;
            inputs.right = false;
        }
    }
   
    InputMode::HandleSocd(inputs);
}

void FGCish::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    // Right hand bottom row
    outputs.a = inputs.b;
    outputs.b = inputs.x;
    outputs.triggerRDigital = inputs.z;
    outputs.triggerLDigital = inputs.up;

    // Right hand top row
    outputs.x = inputs.r;
    outputs.y = inputs.y;
    outputs.buttonR = inputs.lightshield;
    outputs.buttonL = inputs.midshield;

    // Right hand thumb
    outputs.rightStickClick = inputs.a;

    // Left hand row, directions analog
    outputs.leftStickClick = inputs.l;

    // Left hand thumb handled in analog section

    // Toggle socd modes, swap dpad and friends?
    if (toggle_released && inputs.mod_y && inputs.nunchuk_c) {
        fgc_left = !fgc_left;
        toggle_released = false;
    }
    toggle_released = toggle_released || !(inputs.mod_y || inputs.nunchuk_c);

    // Start
    outputs.start = !inputs.nunchuk_c && !inputs.mod_y && inputs.start;

    // Select, MS, or MY + Start for "Reset" in the Lab. Not supported by GameCube adapter.
    outputs.select = !inputs.mod_y && inputs.nunchuk_c && inputs.start;

    // Home not supported by GameCube adapter.
    outputs.home =  inputs.mod_y && !inputs.nunchuk_c && inputs.start;

    if (fgc_left) {
        // Directions
        outputs.dpadLeft = inputs.left;
        outputs.dpadRight = inputs.right;
        outputs.dpadDown = inputs.down;
        outputs.dpadUp = inputs.mod_x;
    } else {
        // MY activates C-Stick to D-Pad conversion.
        if (inputs.nunchuk_c) {
            outputs.dpadLeft = inputs.c_left;
            outputs.dpadRight = inputs.c_right;
            outputs.dpadDown = inputs.c_down;
            outputs.dpadUp = inputs.c_up;
        }
    }
}

void FGCish::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    if (fgc_left) {
        UpdateDirections(
            inputs.nunchuk_c && inputs.c_left,
            inputs.nunchuk_c && inputs.c_right,
            inputs.nunchuk_c && inputs.c_down,
            inputs.nunchuk_c && inputs.c_up,
            !inputs.nunchuk_c && inputs.c_left,
            !inputs.nunchuk_c && inputs.c_right,
            !inputs.nunchuk_c && inputs.c_down,
            !inputs.nunchuk_c && inputs.c_up,
            ANALOG_STICK_MIN,
            ANALOG_STICK_NEUTRAL,
            ANALOG_STICK_MAX,
            outputs
        );
    } else {
        UpdateDirections(
            inputs.left,
            inputs.right,
            inputs.down,
            inputs.mod_x, // Was inputs.up
            !inputs.nunchuk_c && inputs.c_left,
            !inputs.nunchuk_c && inputs.c_right,
            !inputs.nunchuk_c && inputs.c_down,
            !inputs.nunchuk_c && inputs.c_up,
            ANALOG_STICK_MIN,
            ANALOG_STICK_NEUTRAL,
            ANALOG_STICK_MAX,
            outputs
        );
    }

    if (inputs.mod_y) {
        // MY slows down the cursor for easier menu navigation.
        // Menu cursor speed can also be turned down in-game under "Interface" settings.
        // 128 ± 76 results in the slowest cursor that still actuates directional inputs in-game.
        outputs.leftStickX = ANALOG_STICK_NEUTRAL + directions.x * 76;
        outputs.leftStickY = ANALOG_STICK_NEUTRAL + directions.y * 76;

        if (directions.diagonal) {
            // Maintain a consistent cursor velocity when MY is held.
            // ⌊76 × √2/2⌋ = 53
            outputs.leftStickX = ANALOG_STICK_NEUTRAL + directions.x * 53;
            outputs.leftStickY = ANALOG_STICK_NEUTRAL + directions.y * 53;
        }
    }
    

    if (outputs.triggerLDigital) {
        outputs.triggerLAnalog = 140;
    }

    if (outputs.triggerRDigital) {
        outputs.triggerRAnalog = 140;
    }
}
