#pragma once

namespace gui {
  class RenderContext;
}

namespace gfx {
  class Frame;

  class RenderSystem {
  public:
    void render(const Frame& frame);
    void render(const gui::RenderContext& ctx);
  };
}