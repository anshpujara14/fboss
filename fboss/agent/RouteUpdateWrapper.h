/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#pragma once

#include <folly/IPAddress.h>
#include "fboss/agent/if/gen-cpp2/ctrl_types.h"
#include "fboss/agent/state/RouteNextHopEntry.h"
#include "fboss/agent/types.h"

namespace facebook::fboss {

/*
 * Wrapper class to handle route updates and programming across both
 * stand alone RIB and legacy setups
 */
class RouteUpdateWrapper {
  struct AddDelRoutes {
    std::vector<UnicastRoute> toAdd;
    std::vector<IpPrefix> toDel;
  };

 public:
  virtual ~RouteUpdateWrapper() = default;
  void addRoute(
      RouterID id,
      const folly::IPAddress& network,
      uint8_t mask,
      ClientID clientId,
      RouteNextHopEntry entry);

  void delRoute(
      RouterID id,
      const folly::IPAddress& network,
      uint8_t mask,
      ClientID clientId);
  void program();

 private:
  virtual void programLegacyRib() = 0;
  virtual void programStandAloneRib() = 0;

 protected:
  explicit RouteUpdateWrapper(bool isStandaloneRibEnabled)
      : isStandaloneRibEnabled_(isStandaloneRibEnabled) {}
  std::unordered_map<std::pair<RouterID, ClientID>, AddDelRoutes>
      ribRoutesToAddDel_;
  bool isStandaloneRibEnabled_{false};
};
} // namespace facebook::fboss
