#ifndef CPPURSES_PAINTER_COLOR_HPP
#define CPPURSES_PAINTER_COLOR_HPP
#include <cstdint>

namespace cppurses {

namespace detail {
const int k_init_color{239};
}  // namespace detail

/// Colors that can be applied to Glyphs.
enum class Color : std::int16_t {
    Black = detail::k_init_color,
    Dark_red,
    Dark_blue,
    Dark_gray,
    Brown,
    Green,
    Red,
    Gray,
    Blue,
    Orange,
    Light_gray,
    Light_green,
    Violet,
    Light_blue,
    Yellow,
    White
};

namespace detail {

// Used by add_attributes() in brush to overload on different Color types from a
// parameter pack.
enum class BackgroundColor : std::int16_t {
    Black = detail::k_init_color,
    Dark_red,
    Dark_blue,
    Dark_gray,
    Brown,
    Green,
    Red,
    Gray,
    Blue,
    Orange,
    Light_gray,
    Light_green,
    Violet,
    Light_blue,
    Yellow,
    White
};

// Used by add_attributes() in brush to overload on different Color types from a
// parameter pack.
enum class ForegroundColor : std::int16_t {
    Black = detail::k_init_color,
    Dark_red,
    Dark_blue,
    Dark_gray,
    Brown,
    Green,
    Red,
    Gray,
    Blue,
    Orange,
    Light_gray,
    Light_green,
    Violet,
    Light_blue,
    Yellow,
    White
};

}  // namespace detail

/// Converts a Color into a detail::BackgroundColor to be used by Brush.
inline detail::BackgroundColor background(Color c) {
    return static_cast<detail::BackgroundColor>(c);
}

/// Converts a Color into a detail::BackgroundColor to be used by Brush.
inline detail::ForegroundColor foreground(Color c) {
    return static_cast<detail::ForegroundColor>(c);
}

}  // namespace cppurses
#endif  // CPPURSES_PAINTER_COLOR_HPP
