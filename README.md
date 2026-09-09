# Hyprwatermark

A Hyprland plugin that adds customizable image watermarks directly inside application windows.

Unlike desktop overlays or wallpapers, Hyprwatermark attaches the watermark to the window itself, allowing it to move, resize, and switch workspaces with the application.

## Features

* **Image watermark** — Display a custom image inside application windows.
* **Custom positioning** — Position the watermark in the center or any corner of the window.
* **Opacity control** — Independently control the watermark's opacity.
* **Scaling** — Adjust the watermark size with a configurable scale factor.
* **Window-bound** — The watermark follows the window when it moves or resizes.
* **Native rendering** — Uses Hyprland's window decoration and rendering system.
* **Non-invasive** — Does not modify the window's native `active_opacity` or `inactive_opacity`.
* **Rounded corners** — Respects the window's configured corner rounding.

## Motivation

The goal of Hyprwatermark is to provide a persistent, personalized watermark that lives **inside application windows** rather than functioning as a global desktop overlay.

The watermark is designed to:

* Remain attached to the application window.
* Move and resize with the window.
* Follow the window across workspaces.
* Render above the application's content while remaining below popups and menus.
* Maintain its own opacity without affecting Hyprland's native window opacity settings.

Hyprwatermark achieves this through Hyprland's native window decoration system.

## How It Works

Hyprwatermark implements a custom `IHyprWindowDecoration`.

The decoration is rendered using Hyprland's `DECORATION_LAYER_OVER` layer, placing the watermark above the application's content while allowing higher-priority elements such as popups and menus to render above it.

The plugin:

1. Loads the configured watermark image.
2. Creates a custom window decoration.
3. Attaches the decoration to the window.
4. Calculates the watermark's position relative to the window.
5. Renders the image using Hyprland's native rendering passes.
6. Updates its position and size as the window geometry changes.

Because the watermark is implemented as a window decoration, it remains synchronized with the window rather than behaving as an independent desktop overlay.

## Configuration

### Hyprland Config

```conf
plugin:watermark:path = /absolute/path/to/watermark.png
plugin:watermark:opacity = 0.3
plugin:watermark:position = center
plugin:watermark:offset_x = 0
plugin:watermark:offset_y = 0
plugin:watermark:scale = 1.0
```

### Hyprland Lua

```lua
plugin = {
    watermark = {
        path = "/absolute/path/to/watermark.png",
        opacity = 0.1,
        scale = 1.0,
        position = "top-left",
        offset_x = 0,
        offset_y = 0,
    },
}
```

### Options

| Option                 | Description                                                          | Default  |
| ---------------------- | -------------------------------------------------------------------- | -------- |
| `path`                 | Absolute path to the watermark image.                                | —        |
| `opacity`              | Watermark opacity (`0.0`–`1.0`).                                     | —        |
| `scale`                | Image scale multiplier.                                              | `1.0`    |
| `position`             | `center`, `top-left`, `top-right`, `bottom-left`, or `bottom-right`. | `center` |
| `offset_x`             | Horizontal offset in pixels.                                         | `0`      |
| `offset_y`             | Vertical offset in pixels.                                           | `0`      |

## Requirements

* Hyprland `v0.56` or later
* A supported Hyprland plugin environment
* An image file to use as the watermark

## Compatibility

Hyprwatermark has currently been tested on:

* **OS:** Arch Linux
* **Compositor:** Hyprland `v0.56+`

The plugin is currently developed and tested primarily on Arch Linux. Other distributions may work, but have not been explicitly tested.

### FYI

The **Hyprland Lua configuration has been tested and is working**.

The traditional Hyprland `.conf` configuration format has **not yet been tested**. The configuration syntax is included for reference, but compatibility with the `.conf` format has not been verified yet.

If you encounter issues with either configuration method, please open an issue with your Hyprland version and relevant configuration.

## Technical Implementation

* Implements `IHyprWindowDecoration`.
* Uses `DECORATION_LAYER_OVER` for rendering.
* Uses `CTexPassElement` with Hyprland's render-pass system.
* Uses `Hyprgraphics::CImage` for image loading.
* Uses window-relative geometry to keep the watermark synchronized with the window.

## Known Issues

### Damage Tracking

Precise damage tracking is currently being refined.

The plugin tracks the watermark's previous bounding box and uses `damageBox()` to avoid unnecessarily redrawing the entire window. Under certain window movement and resize scenarios, this can result in an after-image or ghosting artifact.

The current implementation may not always map the watermark's bounding box correctly to the coordinate space expected by Hyprland's renderer. Additional damage events may also be required for certain window state changes.

This issue is under active investigation.

## Roadmap

* [ ] Fix precise damage tracking and eliminate after-images.
* [ ] Verify and improve `.conf` configuration support.
* [ ] Improve configuration options.
* [ ] Add per-window filtering.
* [ ] Add additional positioning modes.
* [ ] Improve rendering and damage-tracking performance.

## A Little Note 

This is my first Hyprland plugin and also my first time working with C++ at this level, including working with .hpp files and Hyprland's internal libraries and rendering APIs.

I used GPT as a development aid to understand unfamiliar concepts, APIs, and parts of the Hyprland codebase while building the plugin and implementing the functionality I had in mind.

I've done my best to keep the implementation clean and make everything work reliably, but there is a good chance that parts of the code could be improved or that I may have used patterns that aren't ideal.

If you're experienced with C++ or Hyprland and notice something that could be improved, please feel free to open an issue or submit a PR. I'm very much interested in learning from feedback and improving the codebase.

## License

MIT License. See [LICENSE](LICENSE) for details.
