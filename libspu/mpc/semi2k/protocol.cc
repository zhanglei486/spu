// Copyright 2021 Ant Group Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "libspu/mpc/semi2k/protocol.h"

#include "libspu/mpc/common/ab_api.h"
#include "libspu/mpc/common/ab_kernels.h"
#include "libspu/mpc/common/prg_state.h"
#include "libspu/mpc/common/pub2k.h"
#include "libspu/mpc/semi2k/arithmetic.h"
#include "libspu/mpc/semi2k/boolean.h"
#include "libspu/mpc/semi2k/conversion.h"
#include "libspu/mpc/semi2k/state.h"
#include "libspu/mpc/semi2k/type.h"

namespace spu::mpc {

std::unique_ptr<Object> makeSemi2kProtocol(
    const RuntimeConfig& conf,
    const std::shared_ptr<yacl::link::Context>& lctx) {
  semi2k::registerTypes();

  // FIXME: use same id for different rank
  auto obj =
      std::make_unique<Object>(fmt::format("{}-{}", lctx->Rank(), "SEMI2K"));

  // add communicator
  obj->addState<Communicator>(lctx);

  // register random states & kernels.
  obj->addState<PrgState>(lctx);

  // add Z2k state.
  obj->addState<Z2kState>(conf.field());

  // register public kernels.
  regPub2kKernels(obj.get());

  // register compute kernels
  regABKernels(obj.get());

  // register arithmetic & binary kernels
  obj->addState<Semi2kState>(conf, lctx);
  obj->regKernel<semi2k::ZeroA>();
  obj->regKernel<semi2k::P2A>();
  obj->regKernel<semi2k::A2P>();
  obj->regKernel<semi2k::NotA>();
  obj->regKernel<semi2k::AddAP>();
  obj->regKernel<semi2k::AddAA>();
  obj->regKernel<semi2k::MulAP>();
  obj->regKernel<semi2k::MulAA>();
  obj->regKernel<semi2k::MatMulAP>();
  obj->regKernel<semi2k::MatMulAA>();
  obj->regKernel<semi2k::LShiftA>();
  // obj->regKernel<semi2k::TruncA>();
  obj->regKernel<semi2k::TruncAPr>();

  obj->regKernel<common::AddBB>();
  obj->regKernel<semi2k::CommonTypeB>();
  obj->regKernel<semi2k::CastTypeB>();
  obj->regKernel<semi2k::ZeroB>();
  obj->regKernel<semi2k::B2P>();
  obj->regKernel<semi2k::P2B>();
  obj->regKernel<semi2k::A2B>();

  if (lctx->WorldSize() == 2) {
    obj->regKernel<semi2k::MsbA2B>();
  }
  // obj->regKernel<semi2k::B2A>();
  obj->regKernel<semi2k::B2A_Randbit>();
  obj->regKernel<semi2k::AndBP>();
  obj->regKernel<semi2k::AndBB>();
  obj->regKernel<semi2k::XorBP>();
  obj->regKernel<semi2k::XorBB>();
  obj->regKernel<semi2k::LShiftB>();
  obj->regKernel<semi2k::RShiftB>();
  obj->regKernel<semi2k::ARShiftB>();
  obj->regKernel<semi2k::BitrevB>();
  obj->regKernel<semi2k::BitIntlB>();
  obj->regKernel<semi2k::BitDeintlB>();
  obj->regKernel<semi2k::RandA>();

  return obj;
}

}  // namespace spu::mpc
