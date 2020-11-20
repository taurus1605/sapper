#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// Game sizes
static constexpr int size_easy{ 10 };
static constexpr int size_normal{ 15 };
static constexpr int size_hard{ 20 };
static constexpr int size_easy_bombs{ 15 };
static constexpr int size_normal_bombs{ 33 };
static constexpr int size_hard_bombs{ 77 };

// Window settings
static constexpr int cell_size{ 40 };
static constexpr int margin_lr{ 40 };
static constexpr int margin_tb{ 40 };
static constexpr int sidebar_w{ 250 };
static constexpr int button_h{ 50 };
static constexpr int button_w{ 200 };
static constexpr int game_timer_h{ 30 };
static constexpr int window_w{ cell_size*size_easy + margin_lr + sidebar_w };
static constexpr int window_h{ cell_size*size_easy + 2*margin_tb };

// Images
static const std::string img_lib           { "img/" };
static const std::string img_bomb          { img_lib + "bomb.png" };
static const std::string img_bomb_defused  { img_lib + "bomb_defused.png" };
static const std::string img_bomb_explosed { img_lib + "bomb_explosed.png" };
static const std::string img_error_bomb    { img_lib + "error_bomb.png" };
static const std::string img_btn_pulled    { img_lib + "btn_pulled.png" };
static const std::string img_btn_pushed    { img_lib + "btn_pushed.png" };
static const std::string img_flag          { img_lib + "flag.png" };
static const std::string img_win           { img_lib + "win.png" };
static const std::string img_defeat        { img_lib + "defeat.png" };


#endif // CONSTANTS_H
