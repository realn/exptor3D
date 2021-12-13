#pragma once

namespace gfx {
  class Frame;

  class RenderSystem {
  public:
    void render(const Frame& frame);
  };
}