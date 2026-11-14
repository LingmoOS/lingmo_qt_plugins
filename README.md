# Qt Plugins

Qt plugins that make Qt applications at home in the Lingmo session. The
`lingmo` platform theme makes them follow the system's dark mode, fonts,
icon and cursor themes, open the Lingmo file chooser, and scroll with the
same touchpad feel as Lingmo's own apps. The image format plugins add SVG,
HEIF/HEIC/AVIF and JPEG XL.

The platform theme also installs a Qt Widgets style plugin named `Lingmo`.
It provides palette-aware rounded controls for buttons, menus, fields,
checkboxes, progress bars and scrollbars. `Darkly` remains an alias for
applications that already request that style. KDE window decorations, KDE
configuration modules, DBus integration and animation/shadow helpers are
intentionally excluded.

## Dependencies

`sudo pacman -S gcc cmake qt6-base qt6-declarative qt6-tools wayland wayland-protocols librsvg libheif libjxl`

## Build

```shell
mkdir build
cd build
cmake ..
make
sudo make install
```

For a per-process preview:

```shell
QT_QPA_PLATFORMTHEME=lingmo QT_STYLE_OVERRIDE=Lingmo your-qt-application
```

## License

GPL-3.0-or-later; see [LICENSE](LICENSE).
