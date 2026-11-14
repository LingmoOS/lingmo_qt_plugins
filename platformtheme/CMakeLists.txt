project(platformthemeplugin)
include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets DBus)
find_package(Qt6Gui CONFIG QUIET COMPONENTS Private)
if(NOT TARGET Qt6::GuiPrivate)
    find_package(Qt6 REQUIRED COMPONENTS GuiPrivate)
endif()
find_package(PkgConfig REQUIRED)

set (SRCS
    main.cpp
    waylandexport.h
    waylandexport.cpp
    platformtheme.h
    platformtheme.cpp
    filedialoghelper.h
    filedialoghelper.cpp
    hintsettings.h
    hintsettings.cpp
    touchpadscrolling.h
    touchpadscrolling.cpp
)

# xdg-foreign lets the portal's file dialog parent itself onto the application
# window. Optional: without it dialogs simply open unparented.
pkg_check_modules(WaylandClient QUIET IMPORTED_TARGET wayland-client)
pkg_check_modules(WaylandProtocols QUIET wayland-protocols>=1.32)
if(WaylandClient_FOUND AND WaylandProtocols_FOUND)
    pkg_get_variable(WAYLAND_PROTOCOLS_DATADIR wayland-protocols pkgdatadir)
    pkg_get_variable(WAYLAND_SCANNER wayland-scanner wayland_scanner)
    set(THEME_PROTOCOL_DIR ${CMAKE_CURRENT_BINARY_DIR}/wayland-protocols)
    file(MAKE_DIRECTORY ${THEME_PROTOCOL_DIR})
    set(xdg_foreign_xml
        ${WAYLAND_PROTOCOLS_DATADIR}/unstable/xdg-foreign/xdg-foreign-unstable-v2.xml)
    add_custom_command(OUTPUT ${THEME_PROTOCOL_DIR}/xdg-foreign-unstable-v2-protocol.c
        COMMAND ${WAYLAND_SCANNER} private-code ${xdg_foreign_xml}
            ${THEME_PROTOCOL_DIR}/xdg-foreign-unstable-v2-protocol.c
        DEPENDS ${xdg_foreign_xml} VERBATIM)
    add_custom_command(OUTPUT ${THEME_PROTOCOL_DIR}/xdg-foreign-unstable-v2-client-protocol.h
        COMMAND ${WAYLAND_SCANNER} client-header ${xdg_foreign_xml}
            ${THEME_PROTOCOL_DIR}/xdg-foreign-unstable-v2-client-protocol.h
        DEPENDS ${xdg_foreign_xml} VERBATIM)
    list(APPEND SRCS
        ${THEME_PROTOCOL_DIR}/xdg-foreign-unstable-v2-protocol.c
        ${THEME_PROTOCOL_DIR}/xdg-foreign-unstable-v2-client-protocol.h)
endif()

add_library(lingmoplatformtheme MODULE ${SRCS})

if(WaylandClient_FOUND AND WaylandProtocols_FOUND)
    target_compile_definitions(lingmoplatformtheme PRIVATE LINGMO_PLATFORMTHEME_WAYLAND)
    target_include_directories(lingmoplatformtheme PRIVATE ${THEME_PROTOCOL_DIR})
    target_link_libraries(lingmoplatformtheme PRIVATE PkgConfig::WaylandClient)
endif()

target_compile_definitions(lingmoplatformtheme
    PRIVATE
        "QT_NO_FOREACH"
)

target_link_libraries(lingmoplatformtheme PRIVATE
    Lingmo::Scrolling
    Lingmo::ScrollingWidgets
    Qt6::GuiPrivate
    Qt6::Widgets
    Qt6::Core
    Qt6::DBus
)

find_program(QT_PATHS_EXECUTABLE NAMES qtpaths6 REQUIRED)
execute_process(COMMAND "${QT_PATHS_EXECUTABLE}" -query QT_INSTALL_PLUGINS
    OUTPUT_VARIABLE QT_PLUGINS_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
if(QT_PLUGINS_DIR)
    message(STATUS "Qt6 plugin directory:" "${QT_PLUGINS_DIR}")
else()
    message(FATAL_ERROR "Qt6 plugin directory cannot be detected.")
endif()

install(TARGETS lingmoplatformtheme LIBRARY DESTINATION "${QT_PLUGINS_DIR}/platformthemes")

# Darkly's original Qt Widgets style is vendored under darkly_style.  It is
# enabled only when the KDE Frameworks used by the upstream style are present;
# the platform theme itself remains buildable on a plain Qt installation.
option(LINGMO_BUILD_DARKLY_STYLE "Build the vendored Darkly Qt Widgets style" ON)
if(LINGMO_BUILD_DARKLY_STYLE)
    find_package(ECM 5.102 QUIET NO_MODULE)
    find_package(KF6 QUIET COMPONENTS CoreAddons ColorScheme Config GuiAddons I18n IconThemes WindowSystem)
    if(ECM_FOUND AND KF6_FOUND)
        include(GenerateExportHeader)
        include(KDEInstallDirs6)
        include(KDECMakeSettings)
        find_package(Qt6 REQUIRED COMPONENTS Quick QuickWidgets)
        find_package(KF6FrameworkIntegration QUIET CONFIG)
        find_package(KF6KCMUtils QUIET)
        find_package(KF6KirigamiPlatform QUIET)

        set(DARKLY_STYLE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/darkly_style")
        set(DARKLY_STYLE_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/darkly_style")
        file(MAKE_DIRECTORY "${DARKLY_STYLE_BINARY_DIR}")
        configure_file("${DARKLY_STYLE_DIR}/config-darkly.h.cmake"
                       "${DARKLY_STYLE_BINARY_DIR}/config-darkly.h")

        set(DARKLY_STYLE_SRCS
            darkly_style/animations/darklyanimation.cpp
            darkly_style/animations/darklyanimations.cpp
            darkly_style/animations/darklyanimationdata.cpp
            darkly_style/animations/darklybaseengine.cpp
            darkly_style/animations/darklybusyindicatordata.cpp
            darkly_style/animations/darklybusyindicatorengine.cpp
            darkly_style/animations/darklydialdata.cpp
            darkly_style/animations/darklydialengine.cpp
            darkly_style/animations/darklyenabledata.cpp
            darkly_style/animations/darklygenericdata.cpp
            darkly_style/animations/darklyheaderviewdata.cpp
            darkly_style/animations/darklyheaderviewengine.cpp
            darkly_style/animations/darklyscrollbardata.cpp
            darkly_style/animations/darklyscrollbarengine.cpp
            darkly_style/animations/darklyspinboxdata.cpp
            darkly_style/animations/darklyspinboxengine.cpp
            darkly_style/animations/darklystackedwidgetdata.cpp
            darkly_style/animations/darklystackedwidgetengine.cpp
            darkly_style/animations/darklytabbardata.cpp
            darkly_style/animations/darklytabbarengine.cpp
            darkly_style/animations/darklytoolboxengine.cpp
            darkly_style/animations/darklytransitiondata.cpp
            darkly_style/animations/darklytransitionwidget.cpp
            darkly_style/animations/darklywidgetstatedata.cpp
            darkly_style/animations/darklywidgetstateengine.cpp
            darkly_style/darklyaddeventfilter.cpp
            darkly_style/darklyblurhelper.cpp
            darkly_style/darklyboxshadowrenderer.cpp
            darkly_style/darklyframeshadow.cpp
            darkly_style/darklyhelper.cpp
            darkly_style/darklymdiwindowshadow.cpp
            darkly_style/darklymnemonics.cpp
            darkly_style/darklypropertynames.cpp
            darkly_style/darklyshadowhelper.cpp
            darkly_style/darklysplitterproxy.cpp
            darkly_style/darklystyle.cpp
            darkly_style/darklystyleplugin.cpp
            darkly_style/darklytileset.cpp
            darkly_style/darklytoolsareamanager.cpp
            darkly_style/darklywindowmanager.cpp
        )

        kconfig_add_kcfg_files(DARKLY_STYLE_SRCS
            darkly_style/darklysettings.kcfgc
            darkly_style/darklystyleconfigdata.kcfgc)
        add_library(darkly6lingmo MODULE ${DARKLY_STYLE_SRCS})
        target_include_directories(darkly6lingmo PRIVATE
            "${DARKLY_STYLE_DIR}"
            "${DARKLY_STYLE_DIR}/animations"
            "${DARKLY_STYLE_BINARY_DIR}")
        target_compile_definitions(darkly6lingmo PRIVATE
            DARKLY_HAVE_KSTYLE=0 DARKLY_HAVE_QTQUICK=1 DARKLY_HAVE_X11=0 DARKLY_HAVE_KWAYLAND=0)
        target_link_libraries(darkly6lingmo PRIVATE
            Qt6::Core Qt6::Gui Qt6::Widgets Qt6::DBus Qt6::Quick Qt6::QuickWidgets
            KF6::CoreAddons KF6::ColorScheme KF6::ConfigCore KF6::ConfigGui
            KF6::GuiAddons KF6::IconThemes KF6::WindowSystem)
        if(TARGET KF6::Style)
            target_link_libraries(darkly6lingmo PRIVATE KF6::Style)
        endif()
        install(TARGETS darkly6lingmo LIBRARY DESTINATION "${QT_PLUGINS_DIR}/styles")
    else()
        message(STATUS "Darkly style disabled: ECM and KF6 development packages were not found")
    endif()
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
