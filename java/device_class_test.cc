/******************************************************************************
 *
 *  Copyright 2014 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
#include "btcore/include/device_class.h"
#include <arpa/inet.h>
#include <gtest/gtest.h>
static ::testing::AssertionResult check_bitfield(const char* m_expr, const char* n_expr, int m,
                                                 int n) {
  if (m == n) {
    return ::testing::AssertionSuccess();
  }
  std::stringstream ss;
  ss.str("");
  ss << std::showbase << std::hex << std::setw(8) << std::setfill('0') << m;
  std::string expected_str = ss.str();
  ss.str("");
  ss << std::showbase << std::hex << std::setw(8) << std::setfill('0') << n;
  std::string actual_str = ss.str();
  return ::testing::AssertionFailure()
         << m_expr << " and " << n_expr << " ( " << expected_str << " vs " << actual_str << " )";
}
class DeviceClassTest : public ::testing::Test {};
TEST_F(DeviceClassTest, cod_sizeof) {
  uint8_t dc_stream[] = {0x00, 0x00, 0x00, 0x00};
  bt_device_class_t dc0;
  device_class_from_stream(&dc0, dc_stream);
  EXPECT_EQ((size_t)3, sizeof(dc0));
}
TEST_F(DeviceClassTest, simple) {
  uint8_t dc_stream[][sizeof(bt_device_class_t)] = {
          {0x00, 0x00, 0x00}, {0xff, 0xff, 0xff}, {0xaa, 0x55, 0xaa},
          {0x01, 0x23, 0x45}, {0x20, 0x07, 0x14},
  };
  for (size_t i = 0; i < sizeof(dc_stream) / sizeof(bt_device_class_t); i++) {
    bt_device_class_t dc;
    device_class_from_stream(&dc, (uint8_t*)&dc_stream[i]);
    uint8_t* to_stream = (uint8_t*)&dc;
    EXPECT_PRED_FORMAT2(check_bitfield, (unsigned)dc_stream[i][0], to_stream[0]);
    EXPECT_PRED_FORMAT2(check_bitfield, (unsigned)dc_stream[i][1], to_stream[1]);
    EXPECT_PRED_FORMAT2(check_bitfield, (unsigned)dc_stream[i][2], to_stream[2]);
  }
}
TEST_F(DeviceClassTest, to_stream) {
  {
    bt_device_class_t dc;
    uint8_t dc_stream0[] = {0x00, 0x00, 0x00, 0xaa};
    device_class_from_stream(&dc, dc_stream0);
    uint8_t dc_stream1[] = {0x00, 0x00, 0x00, 0x00};
    int rc = device_class_to_stream(&dc, dc_stream1, sizeof(dc_stream1));
    EXPECT_EQ(3, rc);
