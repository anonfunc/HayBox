#ifndef _MODES_FGCISH_HPP
#define _MODES_FGCISH_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class FGCish : public ControllerMode {
  public:
    FGCish(socd::SocdType socd_type);
  private:
    bool fgc_left;
    bool toggle_released;
    void HandleSocd(InputState &inputs);

  protected:
    virtual void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    virtual void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);
};

#endif
