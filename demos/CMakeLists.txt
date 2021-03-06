add_executable(demos EXCLUDE_FROM_ALL "")

# CHESS
# TODO can you include subdirectory instead?
target_sources(demos PRIVATE
    chess/src/chessboard_widget.cpp
    chess/src/position.cpp
    chess/src/chess_engine.cpp
    chess/src/rules.cpp
    chess/src/state.cpp
    chess/src/chessboard_borders.cpp
    chess/src/chess_ui.cpp
    chess/src/side_pane.cpp
    chess/src/settings_pane.cpp
    chess/src/lower_pane.cpp
    chess/src/move_input.cpp
    chess/src/piece.cpp
    chess/src/move.cpp
    chess/src/standard_rules.cpp
    chess/src/no_rules.cpp
    chess/src/shared_user_input.cpp
    chess/src/player_human.cpp
    chess/src/chess_event_loop.cpp
    chess/src/chess_move_request_event.cpp
)

# GLYPH PAINT
target_sources(demos PRIVATE
    glyph_paint/glyph_paint.cpp
    glyph_paint/paint_area.cpp
    glyph_paint/side_pane.cpp
    glyph_paint/attribute_box.cpp
    glyph_paint/options_box.cpp
    glyph_paint/populated_glyph_stack.cpp
)

# ANIMATION
target_sources(demos PRIVATE
    animation/animated_widget.cpp
)

# NOTEPAD / MAIN MENU
target_sources(demos PRIVATE
    demo.cpp
    main_menu.cpp
    notepad/notepad.cpp
)

find_package(Threads REQUIRED)

target_link_libraries(demos PRIVATE cppurses ${CMAKE_THREAD_LIBS_INIT})
target_compile_features(demos PRIVATE cxx_std_14)

