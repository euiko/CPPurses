#include <cppurses/painter/detail/ncurses_paint_engine.hpp>

#include <array>
#include <clocale>
#include <cstddef>
#include <stdexcept>

#include <signal.h>
#include <string.h>

#include <ncurses.h>

#include <optional/optional.hpp>

#include <cppurses/painter/attribute.hpp>
#include <cppurses/painter/brush.hpp>
#include <cppurses/painter/color.hpp>
#include <cppurses/painter/detail/ncurses_data.hpp>
#include <cppurses/painter/glyph.hpp>
#include <cppurses/system/system.hpp>

#ifndef add_wchstr
#include <cppurses/painter/detail/extended_char.hpp>
#endif

// #define SLOW_PAINT 7

#if defined(SLOW_PAINT)
#include <chrono>
#include <thread>
#endif

namespace {
attr_t color_to_int(cppurses::Color c) {
    return static_cast<attr_t>(c) - cppurses::detail::k_init_color;
}

short find_pair(cppurses::Color foreground, cppurses::Color background) {
    const int color_n{16};
    return color_to_int(background) * color_n + color_to_int(foreground);
}

attr_t attr_to_int(cppurses::Attribute attr) {
    attr_t a = A_NORMAL;
    switch (attr) {
        case cppurses::Attribute::Bold:
            a = A_BOLD;
            break;
        case cppurses::Attribute::Italic:
#if defined(A_ITALIC)
            a = A_ITALIC;
#endif
            break;
        case cppurses::Attribute::Underline:
            a = A_UNDERLINE;
            break;
        case cppurses::Attribute::Standout:
            a = A_STANDOUT;
            break;
        case cppurses::Attribute::Dim:
            a = A_DIM;
            break;
        case cppurses::Attribute::Inverse:
            a = A_REVERSE;
            break;
        case cppurses::Attribute::Invisible:
            a = A_INVIS;
            break;
        case cppurses::Attribute::Blink:
            a = A_BLINK;
            break;
        default:
            throw std::domain_error{"Attribute enum not handled."};
    }
    return a;
}

void initialize_color_pairs() {
    int pair{0};
    const int end_color{cppurses::detail::k_init_color + 16};
    for (int i{cppurses::detail::k_init_color}; i < end_color; ++i) {
        for (int j{cppurses::detail::k_init_color}; j < end_color; ++j) {
            if (pair != 0) {
                ::init_pair(pair, j, i);
            }
            ++pair;
        }
    }
}

}  // namespace

namespace cppurses {
namespace detail {

NCurses_paint_engine::NCurses_paint_engine() {
    setenv("TERM", "xterm-256color", 1);  // TODO not a great idea.
    std::setlocale(LC_ALL, "en_US.UTF-8");
    this->setup_sigwinch();

    ::initscr();
    ::noecho();
    ::keypad(::stdscr, true);
    ::ESCDELAY = 1;
    ::mousemask(ALL_MOUSE_EVENTS, nullptr);
    ::mouseinterval(0);
    ::start_color();
    ::assume_default_colors(k_init_color, k_init_color);
    initialize_color_pairs();
    this->hide_cursor();
}

NCurses_paint_engine::~NCurses_paint_engine() {
    ::endwin();
}

void NCurses_paint_engine::put_glyph(const Glyph& g) {
    // Background Color
    Color back_color{Color::Black};  // intializaed color should never be used
    if (g.brush.background_color()) {
        back_color = *g.brush.background_color();
    }

    // Foreground Color
    Color fore_color{Color::Black};
    if (g.brush.foreground_color()) {
        fore_color = *g.brush.foreground_color();
    }
    short color_pair{find_pair(fore_color, back_color)};

    attr_t attributes{A_NORMAL};
    for (Attribute a : Attribute_list) {
        if (g.brush.has_attribute(a)) {
            attributes |= attr_to_int(a);
        }
    }

#if defined(SLOW_PAINT)
    cchar_t indicate;
    wchar_t symb2[2] = {L'X', L'\0'};
    ::setcchar(&indicate, symb2, A_NORMAL,
               find_pair(Color::White, Color::Black), nullptr);
    ::wadd_wchnstr(::stdscr, &indicate, 1);

    this->refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(SLOW_PAINT));
#endif

#if defined(add_wchstr)
    const wchar_t symb[2] = {g.symbol, L'\0'};
    cchar_t image;
    ::setcchar(&image, symb, attributes, color_pair, nullptr);
    ::wadd_wchnstr(::stdscr, &image, 1);
#else  // no wchar_t support
    bool use_addch{false};
    chtype image{find_chtype(g.symbol, &use_addch)};
    image |= color_pair;
    image |= attributes;
    if (use_addch) {  // For extended chars
        ::waddch(::stdscr, image);
    } else {
        ::waddchnstr(::stdscr, &image, 1);
    }
#endif

#if defined(SLOW_PAINT)
    this->refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(SLOW_PAINT));
#endif
}

void NCurses_paint_engine::put(std::size_t x, std::size_t y, const Glyph& g) {
    this->move_cursor(x, y);
    this->put_glyph(g);
}

void NCurses_paint_engine::move_cursor(std::size_t x, std::size_t y) {
    ::wmove(::stdscr, static_cast<int>(y), static_cast<int>(x));
}

void NCurses_paint_engine::show_cursor(bool show) {
    if (show) {
        ::curs_set(1);
    } else {
        ::curs_set(0);
    }
}
void NCurses_paint_engine::hide_cursor(bool hide) {
    this->show_cursor(!hide);
}

void NCurses_paint_engine::refresh() {
    ::wrefresh(::stdscr);
}

void handle_sigwinch(int sig) {
    NCurses_data::resize_happened = true;
}

void NCurses_paint_engine::setup_sigwinch() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = handle_sigwinch;
    sigaction(SIGWINCH, &sa, NULL);
}

}  // namespace detail
}  // namespace cppurses
