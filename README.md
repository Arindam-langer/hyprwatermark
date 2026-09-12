# Hyprwatermark

![GitHub License](https://img.shields.io/github/license/Arindam-langer/hyprwatermark)
![GitHub last commit](https://img.shields.io/github/last-commit/Arindam-langer/hyprwatermark)
![GitHub issues](https://img.shields.io/github/issues/Arindam-langer/hyprwatermark)

A Hyprland plugin that adds customizable image watermarks directly inside application windows.

Unlike desktop overlays or wallpapers, Hyprwatermark attaches the watermark to the window itself, allowing it to move, resize, and switch workspaces with the application.

## Showcase

![Hyprwatermark Showcase](assets/hyprwatermark-demo.gif)

## Features

* **Image watermark** — Display a custom image inside application windows.
* **Custom positioning** — Position the watermark in the center or any corner of the window.
* **Opacity control** — Independently control the watermark's opacity.
* **Scaling** — Adjust the watermark size with a configurable scale factor.
* **Window-bound** — The watermark follows the window when it moves or resizes.
* **Native rendering** — Uses Hyprland's window decoration and rendering system.
* **Non-invasive** — Does not modify the window's native `active_opacity` or `inactive_opacity`.
* **Per-window exclusion** — Exclude specific applications or wildcard patterns (e.g. games, media players, terminals) from receiving a watermark.

## Motivation

The goal of Hyprwatermark is to provide a persistent, personalized watermark that lives **inside application windows** rather than functioning as a global desktop overlay.

The watermark is designed to:

* Remain attached to the application window.
* Move and resize with the window.
* Follow the window across workspaces.
* Render above the application's content while remaining below popups and menus.
* Maintain its own opacity without affecting Hyprland's native window opacity settings.

Hyprwatermark achieves this through Hyprland's native window decoration system.

## About This Project

This is my first Hyprland plugin, and my first time working with C++ at this level — including `.hpp` files and Hyprland's internal rendering and decoration APIs. I used GPT as a development aid along the way to work through unfamiliar concepts and parts of the Hyprland codebase.

I've tried to keep the implementation clean and reliable, but if you're experienced with C++ or Hyprland and spot something that could be better, please open an issue or PR — I'm glad to learn from it.

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

## Installation

### Method 1: Using hyprpm (Recommended)

`hyprpm` (Hyprland Plugin Manager) is the primary and recommended installation method. It is the easiest way for normal users to install, build, load, and update the plugin.

1. **Add, enable, and load the plugin:**
   ```bash
   hyprpm add https://github.com/Arindam-langer/hyprwatermark
   hyprpm enable hyprwatermark
   hyprpm reload
   ```

2. **Ensure it starts with Hyprland:**
   * **In `hyprland.conf`:**
     ```conf
     exec-once = hyprpm reload -n
     ```
   * **In `hyprland.lua`:**
     ```lua
     hl.exec_cmd("hyprpm reload -n")
     ```

3. **Updating:**
   ```bash
   hyprpm update
   ```

---

### Method 2: Manual Build from Source

Manual building is a supported alternative for users who:

* Don't use `hyprpm`
* Want to build from source
* Are developing or debugging the plugin
* Want to use a local fork or modified version

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Arindam-langer/hyprwatermark.git
   cd hyprwatermark
   ```

2. **Build the shared library (`hyprwatermark.so`):**
   ```bash
   make
   ```
   *(Requires `hyprgraphics` and `hyprland-headers` when building manually).*

3. **Load the plugin:**
   ```bash
   hyprctl plugin load "$(realpath ./hyprwatermark.so)"
   ```

4. **Autoload in configuration:**
   * **In `hyprland.conf`:**
     ```conf
     exec-once = hyprctl plugin load /absolute/path/to/hyprwatermark.so
     ```
   * **In `hyprland.lua`:**
     ```lua
     hl.exec_cmd("hyprctl plugin load /absolute/path/to/hyprwatermark.so")
     ```

## Configuration

Hyprwatermark supports both traditional Hyprland `.conf` syntax and Hyprland's `Lua` configuration format.

### Option 1: Native Hyprland `.conf` (`hyprland.conf`)

You can configure the plugin using block syntax or dotted key syntax:

```conf
plugin {
    watermark {
        # Absolute path to your watermark image (PNG, JPG, WebP supported)
        path = /home/username/Pictures/watermark.png

        # Watermark transparency: 0.0 (completely invisible) to 1.0 (fully opaque)
        opacity = 0.3

        # Size multiplier: 1.0 is original image size, 0.5 is half size
        scale = 0.5

        # Anchor position: center, top-left, top-right, bottom-left, bottom-right
        position = top-left

        # Fine-tune pixel offset from the chosen position (X: horizontal, Y: vertical)
        offset_x = 0
        offset_y = 0

        # Comma-separated list of window classes or wildcard patterns to exclude
        exclude = kitty, firefox, steam_app_*
    }
}
```

### Option 2: Hyprland Lua (`hyprland.lua`)

If you are using Hyprland's native Lua configuration:

```lua
hl.config({
    plugin = {
        watermark = {
            -- Absolute path to your watermark image
            path = "/home/username/Pictures/watermark.png",

            -- Watermark opacity (0.0 to 1.0)
            opacity = 0.1,

            -- Image scale multiplier
            scale = 0.5,

            -- Position: "center", "top-left", "top-right", "bottom-left", "bottom-right"
            position = "top-left",

            -- Pixel offsets
            offset_x = 0,
            offset_y = 0,

            -- Comma-separated window classes or wildcard patterns to exclude
            exclude = "kitty, firefox, steam_app_*",
        },
    },
})
```

### Options Reference

| Option     | Type    | Description                                                                     | Default  |
| ---------- | ------- | ------------------------------------------------------------------------------- | -------- |
| `path`     | string  | Absolute path to the watermark image file.                                      | `""`     |
| `opacity`  | float   | Watermark transparency (`0.0` invisible to `1.0` opaque).                       | `0.5`    |
| `scale`    | float   | Scaling multiplier relative to the source image dimensions.                     | `0.35`   |
| `position` | string  | Anchor location (`center`, `top-left`, `top-right`, `bottom-left`, `bottom-right`). | `center` |
| `offset_x` | int     | Horizontal offset in pixels added to the anchor position.                       | `0`      |
| `offset_y` | int     | Vertical offset in pixels added to the anchor position.                         | `0`      |
| `exclude`  | string  | Comma-separated window classes or wildcard patterns where watermark is disabled.| `""`     |

### Window Class Exclusion

The `exclude` option allows you to prevent watermarks from appearing on specific application windows:

* **Format**: Comma-separated string of window class names or wildcard patterns (e.g. `"kitty, firefox, steam_app_*"`).
* **Whitespace**: Leading and trailing whitespace around each pattern is automatically trimmed.
* **Matching**: Uses POSIX wildcard/glob matching (`fnmatch`):
  * Exact match: `kitty`, `firefox`
  * Wildcard prefix/suffix: `steam_app_*`, `*term*`
* **Default behavior**: When `exclude` is empty or omitted, every mapped window receives a watermark.
* **Finding a Window's Class**:
  You can find the exact class of any open window by running:
  ```bash
  hyprctl activewindow
  ```
  Look for the `class: ...` line in the output. Alternatively, list all windows with:
  ```bash
  hyprctl clients
  ```

## Requirements

- Hyprland 0.56.2 (tested)
- A supported Hyprland plugin environment:
  - `hyprpm` (recommended), or
  - manual build
- `hyprgraphics` and `hyprland-headers` when building manually
- An image file to use as the watermark (PNG, JPG, or WebP)

## Compatibility

Tested with Hyprland 0.56.2.
Because Hyprland plugins depend on internal compositor APIs, compatibility with other versions may require changes.

### Supported Configurations

The plugin supports both:

- **Hyprland Lua** (`hyprland.lua`)
- **Native Hyprland `.conf`** (`hyprland.conf`)

Both formats have been tested with Hyprland 0.56.2.

## Technical Implementation

* Implements `IHyprWindowDecoration`.
* Uses `DECORATION_LAYER_OVER` for rendering above window contents while remaining beneath popups and menus.
* Uses `CTexPassElement` with Hyprland's render-pass system.
* Uses `Hyprgraphics::CImage` for image loading.
* Uses window-relative geometry to keep the watermark synchronized with the window.

## Technical Notes

### Workspace Visibility
During workspace transitions or when windows are mapped on inactive workspaces, the decoration verifies workspace visibility via `w->m_workspace->isVisible()` before submitting render elements to the render pass, preventing watermark ghosting or afterimages on empty workspaces.

### Per-Window Exclusion
Window exclusions are checked dynamically against `w->m_class` using POSIX wildcard matching (`fnmatch`). An `Event::bus()->m_events.window.class_` listener triggers damage recalculations when window classes are assigned or modified asynchronously, and `damageBox()` is dispatched to immediately clear any previously rendered watermark area when an excluded application is detected.

### Damage Tracking
The plugin tracks the watermark's previous bounding box and utilizes `damageBox()` to avoid unnecessary full-window repaints during window moves, resizes, or exclusion transitions.

## License

MIT License. See [LICENSE](LICENSE) for details.
