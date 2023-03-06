// REQUIRES: gpu-intel-pvc || esimd_emulator
// RUN: %clangxx -fsycl %s -o %t.out
// RUN: %GPU_RUN_PLACEHOLDER %t.out

// This test verifies the correctness of LSC intrinsics loading
// from SLM memory.

#include "Inputs/lsc_slm_load.hpp"

// This test verifies the correctness of LSC SLM block load intrinsics.

template <typename T, bool TestMerging> bool test_load(queue Q) {
  constexpr bool Transpose = true;
  constexpr int VS = 1;

  bool Passed = true;
  // test<type, NGroups, LocalSize, VL, VS, Transpose, TestMerging>(Q);
  Passed &= test<T, 1, 1, 4, VS, Transpose, TestMerging>(Q);
  Passed &= test<T, 1, 7, 16, VS, Transpose, TestMerging>(Q);
  Passed &= test<T, 4, 7, 16, VS, Transpose, TestMerging>(Q);
  Passed &= test<T, 16, 8, 8, VS, Transpose, TestMerging>(Q);
  Passed &= test<T, 2, 4, 32, VS, Transpose, TestMerging>(Q);
  Passed &= test<T, 2, 4, 64, VS, Transpose, TestMerging>(Q);
  return Passed;
}

int main() {
  bool Passed = true;

  constexpr bool TestMerging = true;

  auto Q = queue{gpu_selector_v};
  std::cout << "Running lsc_slm_gather() tests on "
            << Q.get_device().get_info<sycl::info::device::name>() << std::endl;

  Passed &= test_load<uint32_t, !TestMerging>(Q);
  Passed &= test_load<uint64_t, !TestMerging>(Q);

  // TODO: Enable the test with 'TestMerging' when lsc_slm_block_load() with
  // 'old_values' operand is supported.
  // Passed &= test_load<uint32_t, TestMerging>(Q);
  // Passed &= test_load<uint64_t, TestMerging>(Q);

  // TODO: Enable the test with 1- and 2-byte element types, with floating point
  // types when lsc_slm_block_load() API is ready.

  std::cout << (Passed ? "Passed" : "FAILED") << std::endl;
  return Passed ? 0 : 1;
}
