// All the overload we need for the project.
// We keep them all in zombie.h and zombie.cpp to induce minimum change to the buildsystem. For now.
// Note that we still have to modify some files in skia itself, as skia is not THAT extensible.

/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "src/gpu/ganesh/gl/GrGLGpu.h"

#include <iostream>

#define NotImplemented() { \
  std::cout << "function not implemented!" << std::endl; \
  SkASSERT(false); \
}

struct ZombieGrRenderTarget : GrRenderTarget {
  ZombieGrRenderTarget(GrGpu* gpu, GrRenderTarget* tgt) :
    GrSurface(gpu, tgt->dimensions(), tgt->fIsProtected, tgt->getLabel()),
    GrRenderTarget(gpu, tgt->dimensions(), tgt->numSamples(), tgt->fIsProtected, tgt->getLabel(), tgt->getStencilAttachmentRefCnt())
    { }

  bool alwaysClearStencil() const {
    NotImplemented();
    return false;
  }

  GrBackendRenderTarget getBackendRenderTarget() const override { NotImplemented(); };

  bool canAttemptStencilAttachment(bool useMSAASurface) const override {
    NotImplemented();
    return true;
  };

  bool completeStencilAttachment(GrAttachment* stencil, bool useMSAASurface) override {
    NotImplemented();
    return true;
  };

  size_t onGpuMemorySize() const override {
    NotImplemented();
  }

  void onSetLabel() override {
    NotImplemented();
  }

  GrBackendFormat backendFormat() const override {
    NotImplemented();
  }

};

// The correct way to do this seems to be two class,
// one inherited from GrRenderTarget and one that dont.
// by doing some checking on asRenderTarget() it will be the same,
// unless ppl dynamic_cast.
// let's hope they dont.
struct ZombieGrTexture : GrTexture {
  sk_sp<GrTexture> txt;
  sk_sp<GrRenderTarget> tgt;
  // If I get it correctly, label get passed into opengl, and does not have their own special semantic in skia, so we dont need to make new label.
  // todo(M.K.):
  // we may have to touch mipmapstatus, as it allocate.
  ZombieGrTexture(GrGpu* gpu, const sk_sp<GrTexture>& txt) :
    GrSurface(gpu, txt->dimensions(), txt->fIsProtected, txt->getLabel()),
    GrTexture(gpu, txt->dimensions(), txt->fIsProtected, txt->textureType(), txt->mipmapStatus(), txt->getLabel()),
    txt(txt) {
      GrRenderTarget* rawTgt = txt->asRenderTarget();
      if (rawTgt) {
        tgt = sk_make_sp<ZombieGrRenderTarget>(gpu, rawTgt);
        std::cout << "zombietexture created (renderable)" << std::endl;
      } else {
        std::cout << "zombietexture created (nonrenderable)" << std::endl;
      }
    }

  void onSetLabel() override {
    NotImplemented();
  }


  GrBackendFormat backendFormat() const override {
    return txt->backendFormat();
  }

  GrBackendTexture getBackendTexture() const override {
    NotImplemented();
  }

  void textureParamsModified() override {
    NotImplemented();
  }

  bool onStealBackendTexture(GrBackendTexture*, SkImage::BackendTextureReleaseProc*) override {
    NotImplemented();
    return true;
  }

  void computeScratchKey(skgpu::ScratchKey*) const override {
    NotImplemented();
  }

  size_t onGpuMemorySize() const override {
    NotImplemented();
  }

  GrRenderTarget* asRenderTarget() override {
    if (tgt) {
      return tgt.get();
    } else {
      return nullptr;
    }
  }

  const GrRenderTarget* asRenderTarget() const override {
    if (tgt) {
      return tgt.get();
    } else {
      return nullptr;
    }
  }

  // todo(M.K.): we might want to override this
  // virtual void onSetRelease(sk_sp<skgpu::RefCntedCallback>) {}

  // todo(M.K.): is this correct?
  void onAbandon() override {
    GrTexture::onAbandon();
  }

  // todo(M.K.): is this correct?
  void onRelease() override {
    GrTexture::onRelease();
  }
};

struct ZombieGrCaps : GrCaps {
  sk_sp<const GrCaps> caps;

  ZombieGrCaps(const sk_sp<const GrCaps>& caps, const GrContextOptions& options) : GrCaps(options), caps(caps) {
    fShaderCaps = std::make_unique<GrShaderCaps>(*caps->fShaderCaps);
  }

  bool isFormatSRGB(const GrBackendFormat&) const override {
    NotImplemented();
  }

  bool isFormatTexturable(const GrBackendFormat&, GrTextureType) const override {
    NotImplemented();
    return true;
  }

  DstCopyRestrictions getDstCopyRestrictions(const GrRenderTargetProxy* src,
                                                     GrColorType ct) const override {
    NotImplemented();
  }

  void addExtraSamplerKey(skgpu::KeyBuilder*,
                          GrSamplerState,
                          const GrBackendFormat&) const override {
    NotImplemented();
  }

  bool dmsaaResolveCanBeUsedAsTextureInSameRenderPass() const override {
    NotImplemented();
    return true;
  }

  bool onSupportsDynamicMSAA(const GrRenderTargetProxy*) const override {
    NotImplemented();
    return false;
  }

  void onApplyOptionsOverrides(const GrContextOptions&) override {
    NotImplemented();
  }

  void onDumpJSON(SkJSONWriter*) const override {
    NotImplemented();
  }

  bool onIsWindowRectanglesSupportedForRT(const GrBackendRenderTarget&) const override {
    NotImplemented();
    return true;
  }

  GrDstSampleFlags onGetDstSampleFlagsForProxy(const GrRenderTargetProxy*) const override {
    NotImplemented();
    return GrDstSampleFlags::kNone;
  }

  bool isFormatCopyable(const GrBackendFormat&) const override {
    NotImplemented();
    return true;
  }

  int maxRenderTargetSampleCount(const GrBackendFormat&) const override { NotImplemented(); }

  bool isFormatAsColorTypeRenderable(GrColorType, const GrBackendFormat&, int) const override { NotImplemented(); }

  bool isFormatRenderable(const GrBackendFormat&, int) const override { NotImplemented(); }

  int getRenderTargetSampleCount(int, const GrBackendFormat&) const override { NotImplemented(); }

  SurfaceReadPixelsSupport surfaceSupportsReadPixels(const GrSurface*) const override { NotImplemented(); }

  SupportedWrite supportedWritePixelsColorType(GrColorType, const GrBackendFormat&, GrColorType) const override { NotImplemented(); }

  GrBackendFormat getBackendFormatFromCompressionType(SkImage::CompressionType) const override { NotImplemented(); }

  skgpu::Swizzle getWriteSwizzle(const GrBackendFormat&, GrColorType) const override { NotImplemented(); }

  uint64_t computeFormatKey(const GrBackendFormat&) const override { NotImplemented(); }

  GrProgramDesc makeDesc(GrRenderTarget*, const GrProgramInfo&, ProgramDescOverrideFlags) const override { NotImplemented(); }

  std::vector<TestFormatColorTypeCombination> getTestingCombinations() const override { NotImplemented(); }

  bool onSurfaceSupportsWritePixels(const GrSurface*) const override { NotImplemented(); }


  bool onCanCopySurface(const GrSurfaceProxy*, const GrSurfaceProxy*, const SkIRect&, const SkIPoint&) const override { NotImplemented(); }

  GrBackendFormat onGetDefaultBackendFormat(GrColorType ct) const override {
    return caps->onGetDefaultBackendFormat(ct);
  }

  bool onAreColorTypeAndFormatCompatible(GrColorType, const GrBackendFormat&) const override { NotImplemented(); }

  SupportedRead onSupportedReadPixelsColorType(GrColorType, const GrBackendFormat&, GrColorType) const override { NotImplemented(); }

  skgpu::Swizzle onGetReadSwizzle(const GrBackendFormat&, GrColorType) const  override { NotImplemented(); }
};

struct ZombieGrGLGpu : GrGpu {
  sk_sp<GrGpu> gpu;

 ZombieGrGLGpu(const sk_sp<GrGpu>& gpu, const GrContextOptions& options) : GrGpu(gpu->getContext()), gpu(gpu) {
    // todo(M.K.): we might want to remove some capabilities.
    this->initCapsAndCompiler(sk_make_sp<ZombieGrCaps>(gpu->refCaps(), options));
  }

  ~ZombieGrGLGpu() { }

  static sk_sp<GrGpu> Make(sk_sp<const GrGLInterface>, const GrContextOptions&, GrDirectContext*);

  bool onSubmitToGpu(bool syncCpu) override { NotImplemented(); }

  GrOpsRenderPass* onGetOpsRenderPass(GrRenderTarget*,
                                      bool useMultisampleFBO,
                                      GrAttachment*,
                                      GrSurfaceOrigin,
                                      const SkIRect&,
                                      const GrOpsRenderPass::LoadAndStoreInfo&,
                                      const GrOpsRenderPass::StencilLoadAndStoreInfo&,
                                      const SkTArray<GrSurfaceProxy*, true>& sampledProxies,
                                      GrXferBarrierFlags renderPassXferBarriers) override { NotImplemented(); }

  void onResolveRenderTarget(GrRenderTarget* target, const SkIRect& resolveRect) override { NotImplemented(); }

  bool onRegenerateMipMapLevels(GrTexture*) override {
    NotImplemented();
    return true;
  }

  bool onCopySurface(GrSurface* dst, GrSurface* src, const SkIRect& srcRect,
                     const SkIPoint& dstPoint) override { NotImplemented(); }

  bool onTransferFromBufferToBuffer(sk_sp<GrGpuBuffer> src,
                                    size_t srcOffset,
                                    sk_sp<GrGpuBuffer> dst,
                                    size_t dstOffset,
                                    size_t size) override {
    NotImplemented();
    return true;
  }

  bool onTransferPixelsTo(GrTexture*,
                          SkIRect,
                          GrColorType textureColorType,
                          GrColorType bufferColorType,
                          sk_sp<GrGpuBuffer>,
                          size_t offset,
                          size_t rowBytes) override {
    NotImplemented();
    return true;
  }

  bool onTransferPixelsFrom(GrSurface*,
                            SkIRect,
                            GrColorType surfaceColorType,
                            GrColorType bufferColorType,
                            sk_sp<GrGpuBuffer>,
                            size_t offset) override {
    NotImplemented();
    return true;
  }

  bool onReadPixels(GrSurface*,
                    SkIRect,
                    GrColorType surfaceColorType,
                    GrColorType dstColorType,
                    void*,
                    size_t rowBytes) override {
    NotImplemented();
    return true;
  }

  bool onWritePixels(GrSurface*,
                     SkIRect,
                     GrColorType surfaceColorType,
                     GrColorType srcColorType,
                     const GrMipLevel[],
                     int mipLevelCount,
                     bool prepForTexSampling) override {
    NotImplemented();
    return true;
  }

  bool onUpdateCompressedBackendTexture(const GrBackendTexture&,
                                        sk_sp<skgpu::RefCntedCallback> finishedCallback,
                                        const void* data,
                                        size_t length) override {
    NotImplemented();
    return true;
  }

  // todo(M.K.): what does this do? does this unbind?
  void onResetContext(uint32_t resetBits) override {
    gpu->onResetContext(resetBits);
  }

  void onResetTextureBindings() override { NotImplemented(); }

  void xferBarrier(GrRenderTarget*, GrXferBarrierType) override { NotImplemented(); }

  sk_sp<GrTexture> onCreateTexture(SkISize dimensions,
                                   const GrBackendFormat& f,
                                   GrRenderable r,
                                   int renderTargetSampleCnt,
                                   SkBudgeted b,
                                   GrProtected p,
                                   int mipLevelCount,
                                   uint32_t levelClearMask,
                                   std::string_view label) override {
    return sk_make_sp<ZombieGrTexture>(this, gpu->onCreateTexture(dimensions, f, r, renderTargetSampleCnt, b, p, mipLevelCount, levelClearMask, label));
  }

  sk_sp<GrTexture> onCreateCompressedTexture(SkISize dimensions,
                                             const GrBackendFormat&,
                                             SkBudgeted,
                                             GrMipmapped,
                                             GrProtected,
                                             const void* data, size_t dataSize) override { NotImplemented(); }

  sk_sp<GrGpuBuffer> onCreateBuffer(size_t size, GrGpuBufferType, GrAccessPattern) override { NotImplemented(); }

  sk_sp<GrTexture> onWrapBackendTexture(const GrBackendTexture&,
                                        GrWrapOwnership,
                                        GrWrapCacheable,
                                        GrIOType) override { NotImplemented(); }

  sk_sp<GrTexture> onWrapCompressedBackendTexture(const GrBackendTexture&,
                                                  GrWrapOwnership,
                                                  GrWrapCacheable) override { NotImplemented(); }

  sk_sp<GrTexture> onWrapRenderableBackendTexture(const GrBackendTexture&,
                                                  int sampleCnt,
                                                  GrWrapOwnership,
                                                  GrWrapCacheable) override { NotImplemented(); }

  sk_sp<GrRenderTarget> onWrapBackendRenderTarget(const GrBackendRenderTarget&) override { NotImplemented(); }

  GrBackendTexture onCreateBackendTexture(SkISize dimensions,
                                          const GrBackendFormat&,
                                          GrRenderable,
                                          GrMipmapped,
                                          GrProtected) override { NotImplemented(); }

  GrBackendTexture onCreateCompressedBackendTexture(SkISize dimensions,
                                                    const GrBackendFormat&,
                                                    GrMipmapped,
                                                    GrProtected) override { NotImplemented(); }

  bool onClearBackendTexture(const GrBackendTexture&,
                             sk_sp<skgpu::RefCntedCallback> finishedCallback,
                             std::array<float, 4> color) override { NotImplemented(); }

  sk_sp<GrAttachment> makeStencilAttachment(const GrBackendFormat& colorFormat,
                                            SkISize dimensions, int numStencilSamples) override { NotImplemented(); }

  sk_sp<GrAttachment> makeMSAAAttachment(SkISize dimensions,
                                         const GrBackendFormat& format,
                                         int numSamples,
                                         GrProtected isProtected,
                                         GrMemoryless) override { NotImplemented(); }

  void deleteBackendTexture(const GrBackendTexture&) override { NotImplemented(); }

  bool compile(const GrProgramDesc&, const GrProgramInfo&) override { NotImplemented(); }

  GrBackendFormat getPreferredStencilFormat(const GrBackendFormat& format) override { NotImplemented(); }

  bool isTestingOnlyBackendTexture(const GrBackendTexture&) const override { NotImplemented(); }

  GrBackendRenderTarget createTestingOnlyBackendRenderTarget(SkISize dimensions,
                                                             GrColorType,
                                                             int sampleCnt,
                                                             GrProtected) override { NotImplemented(); }

  void deleteTestingOnlyBackendRenderTarget(const GrBackendRenderTarget&) override { NotImplemented(); }

  void submit(GrOpsRenderPass* renderPass) override { NotImplemented(); }

  GrFence SK_WARN_UNUSED_RESULT insertFence() override { NotImplemented(); }

  bool waitFence(GrFence) override {
    NotImplemented();
    return true;
  }

  void deleteFence(GrFence) override { NotImplemented(); }

  std::unique_ptr<GrSemaphore> SK_WARN_UNUSED_RESULT makeSemaphore(bool isOwned) override { NotImplemented(); }

  std::unique_ptr<GrSemaphore> wrapBackendSemaphore(const GrBackendSemaphore&,
                                                    GrSemaphoreWrapType,
                                                    GrWrapOwnership) override { NotImplemented(); }

  void insertSemaphore(GrSemaphore* semaphore) override { NotImplemented(); }

  void waitSemaphore(GrSemaphore* semaphore) override { NotImplemented(); }

  void checkFinishProcs() override { NotImplemented(); }

  void finishOutstandingGpuWork() override { NotImplemented(); }

  std::unique_ptr<GrSemaphore> prepareTextureForCrossContextUsage(GrTexture*) override { NotImplemented(); }

  void addFinishedProc(GrGpuFinishedProc finishedProc,
                       GrGpuFinishedContext finishedContext) override { NotImplemented(); }

  GrThreadSafePipelineBuilder* pipelineBuilder() override {
    return gpu->pipelineBuilder();
  }

  sk_sp<GrThreadSafePipelineBuilder> refPipelineBuilder() override {
    return gpu->refPipelineBuilder();
  }

  void disconnect(DisconnectType) override { NotImplemented(); }

  bool precompileShader(const SkData& key, const SkData& data) override {
    NotImplemented();
    return true;
  }
};
