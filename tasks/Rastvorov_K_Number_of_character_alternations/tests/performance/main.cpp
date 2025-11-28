#include <gtest/gtest.h>

#include "Rastvorov_K_Number_of_character_alternations/common/include/common.hpp"
#include "Rastvorov_K_Number_of_character_alternations/mpi/include/ops_mpi.hpp"
#include "Rastvorov_K_Number_of_character_alternations/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace Rastvorov_K_Number_of_character_alternations {

class RastvorovKNumberAfCharacterAlternationsRunPerfTestProcesses
    : public ppc::util::BaseRunPerfTests<InType, OutType> {
  InType input_data_{};

  void SetUp() override {
    input_data_ = 500000;
  }

  bool CheckTestOutputData(OutType&) final { return true; }

  InType GetTestInputData() final { return input_data_; }
};

TEST_P(RastvorovKNumberAfCharacterAlternationsRunPerfTestProcesses, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType,
                                RastvorovKNumberAfCharacterAlternationsMPI,
                                RastvorovKNumberAfCharacterAlternationsSEQ>(
        PPC_SETTINGS_example_processes);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName =
    RastvorovKNumberAfCharacterAlternationsRunPerfTestProcesses::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests,
                         RastvorovKNumberAfCharacterAlternationsRunPerfTestProcesses,
                         kGtestValues,
                         kPerfTestName);

}  // namespace Rastvorov_K_Number_of_character_alternations
