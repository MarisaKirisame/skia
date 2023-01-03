#include "zombie.h"

#include "include/gpu/GrDirectContext.h"
#include "src/gpu/ganesh/GrGpu.h"
#include "src/gpu/ganesh/gl/GrGLGpu.h"

#include <iostream>

sk_sp<GrDirectContext> GrDirectContext::MakeZombieGL(sk_sp<const GrGLInterface> glInterface,
                                                     const GrContextOptions& options) {
  sk_sp<GrDirectContext> direct(new GrDirectContext(GrBackendApi::kZombieGL, options));
  direct->fGpu = ZombieGrGLGpu::Make(std::move(glInterface), options, direct.get());
  if (!direct->fGpu) {
    std::cout << "cannot make grglgpu" << std::endl;
  }
  if (!direct->init()) {
    return nullptr;
  }
  return direct;
}

sk_sp<GrDirectContext> GrDirectContext::MakeZombieGL(sk_sp<const GrGLInterface> glInterface) {
  GrContextOptions defaultOptions;
  return MakeZombieGL(std::move(glInterface), defaultOptions);
}

sk_sp<GrGpu> ZombieGrGLGpu::Make(sk_sp<const GrGLInterface> interface, const GrContextOptions& options,
                                 GrDirectContext* direct) {
  return sk_sp<GrGpu>(new ZombieGrGLGpu(GrGLGpu::Make(interface, options, direct), options));
}
