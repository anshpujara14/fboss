// Copyright 2004-present Facebook. All Rights Reserved.

#include "fboss/agent/hw/sai/api/LagApi.h"
#include "fboss/agent/hw/sai/api/PortApi.h"
#include "fboss/agent/hw/sai/fake/FakeSai.h"

#include <folly/logging/xlog.h>

#include <gtest/gtest.h>

using namespace facebook::fboss;

class LagApiTest : public ::testing::Test {
 public:
  void SetUp() override {
    fs = FakeSai::getInstance();
    sai_api_initialize(0, nullptr);
    lagApi = std::make_unique<LagApi>();
  }

  LagSaiId createLag() {
    return lagApi->create<SaiLagTraits>({}, switchid);
  }

  void removeLag(LagSaiId lag) {
    return lagApi->remove(lag);
  }

  bool hasLag(LagSaiId lag) {
    return fs->lagManager.exists(lag);
  }

  std::vector<sai_object_id_t> getMembers(LagSaiId lag) {
    return lagApi->getAttribute(lag, SaiLagTraits::Attributes::PortList{});
  }

  LagMemberSaiId createLagMember(sai_object_id_t lag, sai_object_id_t port) {
    return lagApi->create<SaiLagMemberTraits>({lag, port}, switchid);
  }

  bool hasLagMember(LagSaiId lagId, LagMemberSaiId lagMemberId) {
    auto members = getMembers(lagId);
    std::set<LagMemberSaiId> membersSet;
    std::for_each(
        members.begin(), members.end(), [&membersSet](sai_object_id_t id) {
          membersSet.insert(static_cast<LagMemberSaiId>(id));
        });
    return membersSet.find(lagMemberId) != membersSet.end();
  }

  void removeLagMember(LagMemberSaiId member) {
    return lagApi->remove(member);
  }

  void setLabel(LagSaiId lag, std::string label) {
    std::array<char, 32> data{};
    std::copy(std::begin(label), std::end(label), std::begin(data));
    return lagApi->setAttribute(lag, SaiLagTraits::Attributes::Label(data));
  }

  void checkLabel(LagSaiId lag, std::string label) {
    auto attr = lagApi->getAttribute(lag, SaiLagTraits::Attributes::Label{});
    std::string value(attr.data());
    EXPECT_EQ(label, value);
  }

  sai_object_id_t switchid{0};
  std::shared_ptr<FakeSai> fs;
  std::unique_ptr<LagApi> lagApi;
};

TEST_F(LagApiTest, TestApi) {
  /* create */
  auto id0 = createLag();
  auto id1 = createLag();
  EXPECT_NE(id0, id1);
  EXPECT_TRUE(hasLag(id0));
  EXPECT_TRUE(hasLag(id1));

  auto mem00 = createLagMember(id0, 1);
  auto mem01 = createLagMember(id0, 2);
  auto mem02 = createLagMember(id0, 3);
  auto mem03 = createLagMember(id0, 4);

  auto mem10 = createLagMember(id1, 5);
  auto mem11 = createLagMember(id1, 6);
  auto mem12 = createLagMember(id1, 7);
  auto mem13 = createLagMember(id1, 8);

  EXPECT_TRUE(hasLagMember(id0, mem00));
  EXPECT_TRUE(hasLagMember(id0, mem01));
  EXPECT_TRUE(hasLagMember(id0, mem02));
  EXPECT_TRUE(hasLagMember(id0, mem03));

  EXPECT_TRUE(hasLagMember(id1, mem10));
  EXPECT_TRUE(hasLagMember(id1, mem11));
  EXPECT_TRUE(hasLagMember(id1, mem12));
  EXPECT_TRUE(hasLagMember(id1, mem13));

  /* set attribute */
  setLabel(id0, "id0");
  checkLabel(id0, "id0");

  setLabel(id1, "id1");
  checkLabel(id1, "id1");

  /* delete member */
  removeLagMember(mem00);
  removeLagMember(mem10);
  EXPECT_FALSE(hasLagMember(id0, mem00));
  EXPECT_FALSE(hasLagMember(id1, mem10));
}
