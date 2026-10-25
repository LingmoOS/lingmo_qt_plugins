# Qt Plugins

Qt plugins that make Qt applications at home in the Lingmo session. The
`lingmo` platform theme makes them follow the system's dark mode, fonts,
icon and cursor themes, open the Lingmo file chooser, and scroll with the
same touchpad feel as Lingmo's own apps. The image format plugins add SVG,
HEIF/HEIC/AVIF and JPEG XL.

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

## License

GPL-3.0-or-later; see [LICENSE](LICENSE).
