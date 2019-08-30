/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "fboss/agent/hw/sai/api/NextHopApi.h"
#include "fboss/agent/hw/sai/fake/FakeSai.h"
#include "fboss/agent/hw/sai/store/SaiObject.h"

#include <folly/logging/xlog.h>

#include <gtest/gtest.h>

#include <variant>

using namespace facebook::fboss;

class NextHopStoreTest : public ::testing::Test {
 public:
  void SetUp() override {
    fs = FakeSai::getInstance();
    sai_api_initialize(0, nullptr);
    saiApiTable = SaiApiTable::getInstance();
    saiApiTable->queryApis();
  }
  std::shared_ptr<FakeSai> fs;
  std::shared_ptr<SaiApiTable> saiApiTable;

  NextHopSaiId createNextHop(const folly::IPAddress& ip) {
    return saiApiTable->nextHopApi().create2<SaiNextHopTraits>(
        {SAI_NEXT_HOP_TYPE_IP, 42, ip}, 0);
  }
};

TEST_F(NextHopStoreTest, nextHopLoadCtor) {
  auto ip = folly::IPAddress("::");
  auto nextHopSaiId = createNextHop(ip);
  SaiObject<SaiNextHopTraits> obj(nextHopSaiId);
  EXPECT_EQ(obj.adapterKey(), nextHopSaiId);
  EXPECT_EQ(GET_ATTR(NextHop, Ip, obj.attributes()), ip);
}

TEST_F(NextHopStoreTest, nextHopCreateCtor) {
  auto ip = folly::IPAddress("::");
  SaiNextHopTraits::CreateAttributes c{SAI_NEXT_HOP_TYPE_IP, 42, ip};
  SaiNextHopTraits::AdapterHostKey k{42, ip};
  SaiObject<SaiNextHopTraits> obj(k, c, 0);
  EXPECT_EQ(GET_ATTR(NextHop, Ip, obj.attributes()), ip);
}
