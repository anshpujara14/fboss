// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include "fboss/agent/types.h"

namespace facebook::fboss {

class Wedge40LedUtils {
 public:
  enum class LedState : uint32_t {
    OFF = 0x0,
    ON = 0x1,
  };
  static LedState getLEDState(bool up, bool adminUp);
  static int getPortIndex(PortID port);
  static size_t getPortOffset(int index);
  static folly::ByteRange defaultLedCode();
  static std::optional<uint32_t> getLEDProcessorNumber(PortID port);
};

} // namespace facebook::fboss
