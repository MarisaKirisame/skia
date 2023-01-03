#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkPath.h"
#include "include/core/SkPaint.h"
#include "include/core/SkCanvas.h"
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/gl/GrGLInterface.h"

#include "zombie.h"

#include "Zombie/zombie.hpp"

#include <GL/gl.h>
#include <GL/glx.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

void draw(SkCanvas* canvas) {
  const SkScalar scale = 256.0f;
  const SkScalar R = 0.45f * scale;
  const SkScalar TAU = 6.2831853f;
  SkPath path;
  path.moveTo(R, 0.0f);
  for (int i = 1; i < 7; ++i) {
    SkScalar theta = 3 * i * TAU / 7;
    path.lineTo(R * cos(theta), R * sin(theta));
  }
  path.close();
  SkPaint p;
  p.setAntiAlias(true);
  canvas->clear(SK_ColorWHITE);
  canvas->translate(0.5f * scale, 0.5f * scale);
  canvas->drawPath(path, p);
}

void save_result(const sk_sp<SkSurface>& surface, const char* path) {
  sk_sp<SkImage> img(surface->makeImageSnapshot());
  if (!img) { return; }
  sk_sp<SkData> png(img->encodeToData());
  if (!png) { return; }
  SkFILEWStream out(path);
  (void)out.write(png->data(), png->size());
}

void raster(int width, int height,
            void (*draw)(SkCanvas*),
            const char* path) {
  sk_sp<SkSurface> rasterSurface =
    SkSurface::MakeRasterN32Premul(width, height);
  SkCanvas* rasterCanvas = rasterSurface->getCanvas();
  draw(rasterCanvas);
  save_result(rasterSurface, path);
}

#include "tools/sk_app/Window.h"

void gl_init() {
  glfwInit();
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(640, 480, "", NULL, NULL); // offscreen
  glfwMakeContextCurrent(window);
}

void gl_raster(int width, int height, void (*draw)(SkCanvas*), const char* path) {
  static_assert(SK_SUPPORT_GPU);
  // You've already created your OpenGL context and bound it.
  sk_sp<const GrGLInterface> interface = nullptr;
  // Leaving interface as null makes Skia extract pointers to OpenGL functions for the current
  // context in a platform-specific way. Alternatively, you may create your own GrGLInterface and
  // initialize it however you like to attach to an alternate OpenGL implementation or intercept
  // Skia's OpenGL calls.
  sk_sp<GrDirectContext> context = GrDirectContext::MakeGL(interface);
  SkImageInfo info = SkImageInfo:: MakeN32Premul(width, height);
  if (context.get() == nullptr) {
    std::cout << "cannot found context!" << std::endl;
    return;
  }
  sk_sp<SkSurface> gpuSurface(SkSurface::MakeRenderTarget(context.get(), SkBudgeted::kNo, info));
  if (!gpuSurface) {
    SkDebugf("SkSurface::MakeRenderTarget returned null\n");
    return;
  }
  SkCanvas* gpuCanvas = gpuSurface->getCanvas();
  draw(gpuCanvas);
  save_result(gpuSurface, path);
}

void zombie_gl_raster(int width, int height, void (*draw)(SkCanvas*), const char* path) {
  static_assert(SK_SUPPORT_GPU);
  // You've already created your OpenGL context and bound it.
  sk_sp<const GrGLInterface> interface = nullptr;
  // Leaving interface as null makes Skia extract pointers to OpenGL functions for the current
  // context in a platform-specific way. Alternatively, you may create your own GrGLInterface and
  // initialize it however you like to attach to an alternate OpenGL implementation or intercept
  // Skia's OpenGL calls.
  sk_sp<GrDirectContext> context = GrDirectContext::MakeZombieGL(interface);
  SkImageInfo info = SkImageInfo:: MakeN32Premul(width, height);
  if (context.get() == nullptr) {
    std::cout << "cannot found context!" << std::endl;
    return;
  }
  sk_sp<SkSurface> gpuSurface(SkSurface::MakeRenderTarget(context.get(), SkBudgeted::kNo, info));
  if (!gpuSurface) {
    SkDebugf("SkSurface::MakeRenderTarget returned null\n");
    return;
  }
  SkCanvas* gpuCanvas = gpuSurface->getCanvas();
  draw(gpuCanvas);
  save_result(gpuSurface, path);
}

void normal() {
  zombie_link_test();
  raster(400, 400, draw, "cpu/example.png");
  gl_init();
  gl_raster(400, 400, draw, "gl/example.png");
  zombie_gl_raster(400, 400, draw, "zombie_gl/example.png");
}

void debug() {
  zombie_link_test();
  gl_init();
  zombie_gl_raster(400, 400, draw, "zombie_gl/example.png");
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args;
  for (size_t i = 1; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  if (args.size() == 0) {
    normal();
  } else if (args.size() == 1 && args[0] == "debug") {
    debug();
  } else {
    std::cout << "unknown argument: ";
    for (const std::string& str : args) {
      std::cout << str << " ";
    }
    std::cout << std::endl;
  }
}
