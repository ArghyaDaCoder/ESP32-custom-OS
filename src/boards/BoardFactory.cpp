// ============================================================
//  BoardFactory.cpp  –  selects the right IBoardConfig at
//  compile time using the -D BOARD_xxx flag from platformio.ini
//
//  TO ADD A NEW BOARD:
//    1. Create NewBoard.h / NewBoard.cpp  (copy WrovrBoard as template)
//    2. Add  #elif defined(BOARD_MY_NEW_BOARD) ... return new NewBoard()
//    3. Add  [env:my_new_board]  to platformio.ini with -D BOARD_MY_NEW_BOARD
//    That is literally all.
// ============================================================
#include "../hal/IBoardConfig.h"

#if defined(BOARD_WROVER_ILI9341)
    #include "WrovrBoard.h"
    IBoardConfig* Board::create() { return new WrovrBoard(); }

// ── Future boards – uncomment when you have the hardware ─────
// #elif defined(BOARD_S3MINI_GC9A01)
//     #include "S3MiniBoard.h"
//     IBoardConfig* Board::create() { return new S3MiniBoard(); }

#else
    #error "No board selected! Add -D BOARD_xxx to platformio.ini build_flags."
#endif
