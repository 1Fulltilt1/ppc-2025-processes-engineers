#include "Rastvorov_K_Number_of_character_alternations/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <cstddef>
#include <vector>

#include "Rastvorov_K_Number_of_character_alternations/common/include/common.hpp"
#include "util/include/util.hpp"

namespace Rastvorov_K_Number_of_character_alternations {

namespace {

inline int Sign(double x) {
  if (x > 0.0) return 1;
  if (x < 0.0) return -1;
  return 0;
}

inline double GetElement(std::size_t i) {
  if (i % 5 == 0) return 0.0;
  if (i % 2 == 0) return 1.0;
  return -1.0;
}

}  // namespace

RastvorovKNumberAfCharacterAlternationsMPI::RastvorovKNumberAfCharacterAlternationsMPI(const InType& in) {
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
  int rank = 0, size = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const InType n = GetInput();
  if (n <= 0) {
    if (rank == 0) GetOutput() = 0;
    return true;
  }

  const std::size_t total = static_cast<std::size_t>(n);

  std::size_t base = total / static_cast<std::size_t>(size);
  std::size_t rem  = total % static_cast<std::size_t>(size);

  std::size_t begin = 0, end = 0;
  if (static_cast<std::size_t>(rank) < rem) {
    begin = static_cast<std::size_t>(rank) * (base + 1);
    end   = begin + base + 1;
  } else {
    begin = rem * (base + 1) + (static_cast<std::size_t>(rank) - rem) * base;
    end   = begin + base;
  }

  int local_count = 0;
  int first_sign  = 0;
  int last_sign   = 0;

  for (std::size_t i = begin; i < end; ++i) {
    int s = Sign(GetElement(i));
    if (s == 0) continue;

    if (first_sign == 0) first_sign = s;
    if (last_sign != 0 && s != last_sign) ++local_count;
    last_sign = s;
  }

  int local_info[3] = { local_count, first_sign, last_sign };
  std::vector<int> all_info;

  if (rank == 0)
    all_info.resize(static_cast<std::size_t>(size) * 3);

  MPI_Gather(local_info, 3, MPI_INT,
             rank == 0 ? all_info.data() : nullptr, 3, MPI_INT,
             0, MPI_COMM_WORLD);

  if (rank == 0) {
    int global_count = 0;
    int prev_sign = 0;

    for (int p = 0; p < size; ++p) {
      int lc = all_info[3 * p + 0];
      int fs = all_info[3 * p + 1];
      int ls = all_info[3 * p + 2];

      if (prev_sign != 0 && fs != 0 && fs != prev_sign)
        ++global_count;

      global_count += lc;

      if (ls != 0)
        prev_sign = ls;
    }

    GetOutput() = global_count;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  return true;
}

bool RastvorovKNumberAfCharacterAlternationsMPI::PostProcessingImpl() {
  return true;
}

}  // namespace Rastvorov_K_Number_of_character_alternations
