// RUN: %clang --sycl -Xclang -fsycl-int-header=%t.h %s -c -o %T/kernel.spv
// RUN: FileCheck -input-file=%t.h %s

// CHECK:     const kernel_param_desc_t kernel_signatures[] = {
// CHECK-NEXT:  //--- MyKernel
// CHECK-NEXT:  { kernel_param_kind_t::kind_accessor, 2014, 0 },
// CHECK-NEXT:  { kernel_param_kind_t::kind_std_layout, 8, 16 },
// CHECK-NEXT:  { kernel_param_kind_t::kind_std_layout, 24, 32 },
// CHECK-EMPTY:
// CHECK-NEXT:};


// This test checks if compiler accepts structures as kernel parameters.

#include "sycl.hpp"

using namespace cl::sycl;

struct MyNestedStruct {
  int FldArr[1];
  float FldFloat;
};

struct MyStruct {
  int FldInt;
  MyNestedStruct FldStruct;
  int FldArr[3];
};

MyStruct GlobS;

bool test0() {
  MyStruct S = GlobS;
  MyStruct S0 = { 0 };
  {
    buffer<MyStruct, 1> Buf(&S0, range<1>(1));
    queue myQueue;
    myQueue.submit([&](handler &cgh) {
      auto B = Buf.get_access<access::mode::write>(cgh);
      cgh.single_task<class MyKernel>([=] { B[0] = S; });
    });
  }
}

