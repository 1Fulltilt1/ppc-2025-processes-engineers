#include <gtest/gtest.h>

#include "Rastvorov_K_Number_of_character_alternations/common/include/common.hpp"
#include "Rastvorov_K_Number_of_character_alternations/mpi/include/ops_mpi.hpp"
#include "Rastvorov_K_Number_of_character_alternations/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace Rastvorov_K_Number_of_character_alternations {  // NOLINT(readability-identifier-naming)

class RastvorovKNumberAfCharacterAlternationsRunPerfTestProcesses
    : public ppc::util::BaseRunPerfTests<InType, OutType> {
  InType input_data_{};  //

  void SetUp() override {
    input_data_ = 10000000;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    (void)output_data;
    return true;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(RastvorovKNumberAfCharacterAlternationsRunPerfTestProcesses, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, RastvorovKNumberAfCharacterAlternationsMPI,
                                RastvorovKNumberAfCharacterAlternationsSEQ>(PPC_SETTINGS_example_processes);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

INSTANTIATE_TEST_SUITE_P(  // NOLINT
    RunModeTests, RastvorovKNumberAfCharacterAlternationsRunPerfTestProcesses, kGtestValues,
    RastvorovKNumberAfCharacterAlternationsRunPerfTestProcesses::CustomPerfTestName);

}  // namespace Rastvorov_K_Number_of_character_alternations
