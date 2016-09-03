/*
	Colored Text example made by Aurelio Mannara for ctrulib
	This code was modified for the last time on: 12/12/2014 23:00 UTC+1

*/

#include <3ds.h>
#include <string>
#include <iostream>
#include "string_utils.h"

using namespace std;

const int TOP_WIDTH = 50;
const int TOP_LINES = 30;
const int BOTTOM_COLUMNS = 40;
const int BOTTOM_LINES = 30;

int main(int argc, char **argv)
{
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// run once begin
    ///

    //printf("\x1b[15;19HHello World!");
    //printf("\x1b[0;0H\x1b[31;43mTOPLEFT\x1b[0m");
    //printf("\x1b[29;0H\x1b[31;43mBOTLEFT\x1b[0m");



    //error(string("running"));
    string test = "one two three four five six seven eight nine ten eleven tvelve thirteen fourteen fifteen sixteen";
    string _3ds = "The handheld offers new features such as the StreetPass and SpotPass tag modes, powered by Nintendo Network; augmented reality, using its 3D cameras; and Virtual Console, which allows owners to download and play games originally released on older video game systems. It is also pre-loaded with various applications including these: an online distribution store called Nintendo eShop, a social networking service called Miiverse; an Internet Browser; the Netflix, Hulu Plus and YouTube streaming video services; Nintendo Video; a messaging application called Swapnote (known as Nintendo Letter Box in Europe and Australia); and Mii Maker.";
    string text = fmt_text(_3ds, TOP_WIDTH, TEXT_JUSIFY_BOTH);
    cout << text << endl;

    ///
    /// run once end
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    while (aptMainLoop())
    {

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// run loop begin
        ///
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u32 kDown = hidKeysDown();

        if (kDown & KEY_START) break; // break in order to return to hbmenu

        ///
        /// run loop end
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Flush and swap framebuffers
        gfxFlushBuffers();
        gfxSwapBuffers();

        //Wait for VBlank
        gspWaitForVBlank();
    }

    // Exit services
    gfxExit();

    return 0;
}
