//*****************************************************************************
// Copyright 2021 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//*****************************************************************************
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../ov_utils.hpp"
#include "../sequence.hpp"
#include "stateful_test_utils.hpp"

TEST(Sequence, SequenceDisabled) {
    uint64_t sequenceId = 3;
    ovms::Sequence sequence(sequenceId);
    ASSERT_FALSE(sequence.isTerminated());
    sequence.setTerminated();
    ASSERT_TRUE(sequence.isTerminated());
}

TEST(Sequence, UpdateSequenceState) {
    ovms::model_memory_state_t_2 newState;
    DummyStatefulModel model;
    std::vector<float> expectedState{10};
    ov::runtime::InferRequest auxInferRequest = model.createInferRequest_2();

    model.setVariableState_2(auxInferRequest, expectedState);
    ov::runtime::VariableState memoryState = model.getVariableState_2(auxInferRequest);
    newState.push_back(memoryState);
    uint64_t sequenceId = 3;
    ovms::Sequence sequence(sequenceId);
    sequence.updateMemoryState_2(newState);

    const ovms::sequence_memory_state_t_2& sequenceMemoryState = sequence.getMemoryState_2();
    const std::string stateName = model.getStateName();
    ASSERT_TRUE(sequenceMemoryState.count(stateName));

    // std::vector<float> stateBlobSequenceData;
    // stateBlobSequenceData.assign(InferenceEngine::as<ov::runtime::Tensor>(sequenceMemoryState.at(stateName))->rmap().as<float*>(), InferenceEngine::as<ov::runtime::Tensor>(sequenceMemoryState.at(stateName))->rmap().as<float*>() + 1);
    // EXPECT_EQ(stateBlobSequenceData, expectedState);
}
