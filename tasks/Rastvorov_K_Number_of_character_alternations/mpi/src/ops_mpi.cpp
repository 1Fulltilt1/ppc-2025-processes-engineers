#include "Rastvorov_K_Number_of_character_alternations/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <cstddef>
#include <vector>

#include "Rastvorov_K_Number_of_character_alternations/common/include/common.hpp"
#include "util/include/util.hpp"

namespace Rastvorov_K_Number_of_character_alternations {

namespace {

inline int Sign(double x) {
  if (x > 0.0) {
    return 1;
  }
  if (x < 0.0) {
    return -1;
  }
  return 0;
}

inline double GetElement(std::size_t i) {
  if (i % 5 == 0) {
    return 0.0;
  }
  if (i % 2 == 0) {
    return 1.0;
  }
  return -1.0;
}

}  // namespace

RastvorovKNumberAfCharacterAlternationsMPI::RastvorovKNumberAfCharacterAlternationsMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool RastvorovKNumberAfCharacterAlternationsMPI::ValidationImpl() {
  return GetInput() >= 0;
}

bool RastvorovKNumberAfCharacterAlternationsMPI::PreProcessingImpl() {
  GetOutput() = 0;
  return true;
}

bool RastvorovKNumberAfCharacterAlternationsMPI::RunImpl() {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const InType n = GetInput();

  int result = 0;

  if (rank == 0) {
    if (n <= 0) {
      result = 0;
    } else {
      int local_count = 0;
      int last_sign = 0;

      for (std::size_t i = 0; i < static_cast<std::size_t>(n); ++i) {
        int s = Sign(GetElement(i));
        if (s == 0) {
          continue;
        }
        if (last_sign != 0 && last_sign != s) {
          ++local_count;
        }
        last_sign = s;
      }

      result = local_count;
    }
    GetOutput() = result;
  }

  MPI_Bcast(&result, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank != 0) {
    GetOutput() = result;
  }

  return true;
}

bool RastvorovKNumberAfCharacterAlternationsMPI::PostProcessingImpl() {
  return true;
}

}  // namespace Rastvorov_K_Number_of_character_alternations
