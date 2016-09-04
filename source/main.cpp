#include <3ds.h>
#include <string>
#include <iostream>
#include <cstdio>
#include "string_utils.h"
#include "mandel.h"
#include <unistd.h>
using namespace std;

const size_t TOP_WIDTH = 50;
const size_t TOP_HEIGHT = 30;
const size_t BOTTOM_COLUMNS = 40;
const size_t BOTTOM_LINES = 30;

void run_string_test(){
    string test = "one two three\n four five six seven eight nine ten eleven tvelve thirteen fourteen fifteen sixteen";
    string _3ds = "The handheld offers new features such as the StreetPass and SpotPass tag modes, powered by Nintendo Network;\naugmented reality, using its 3D cameras; and Virtual Console, which allows owners to download and play games originally released on older video game systems. It is also pre-loaded with various applications including these: an online distribution store called Nintendo eShop, a social networking service called Miiverse; an Internet Browser; the Netflix, Hulu Plus and YouTube streaming video services; Nintendo Video; a messaging application called Swapnote (known as Nintendo Letter Box in Europe and Australia); and Mii Maker.";
    string text = fmt_text(_3ds, TOP_WIDTH, TEXT_JUSTIFY_BOTH);
    cout << text << endl;
}

void run_mandel(fieldDef field){
    consoleInit(GFX_TOP, NULL);
    double tolerance = 2.0;
    vector<string> pallet = preparePallet(true);

    vector<vector<size_t>> iters = getField(field, 1000, tolerance);

    histogram_acc h = histogram(iters);
    map<size_t, size_t> mapping = buildPalletMappings(h, pallet.size());

    for (size_t i = 0; i < iters.size(); ++i) {
        vector<size_t> line = iters.at(i);

        for (size_t j = 0; j < line.size(); ++j) {
            size_t cell = line.at(j);
            size_t quant = mapping[cell];
            if(quant >= pallet.size()){
                printf("FAIL %d", quant);

            }
            //size_t quant = quantify(cell, 0, 1000, pallet.size());
            //usleep(1000000);
            printf(pallet[quant].c_str());
            gfxFlushBuffers();

        }

    }

}

int main(int argc, char **argv)
{
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// run once begin
    ///
    //vector<string> pallet = preparePallet();
    //for (string s :pallet) {
    //    printf("[%s] \n", s.c_str());
    //}

    double default_width = 3;
    double default_height = 3;

    fieldDef def =  {
            TOP_WIDTH,
            TOP_HEIGHT,
            -2.25,
            -2.25+default_width,
            -1.5,
            -1.5+default_height
    };
    fieldDef field =def;
    double move_factor = 0.5; // of viewport, not absolute
    double zoom_factor = 2; // of viewport, not absolute.


    run_mandel(field);

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

        if(kDown){
            double viewport_width = field.RE_to-field.RE_from;
            double viewport_height = field.IM_to-field.IM_from;
            if (kDown & KEY_RIGHT) {
                field.RE_from += viewport_width * move_factor;
                field.RE_to += viewport_width * move_factor;
            }
            if (kDown & KEY_LEFT) {
                field.RE_from -= viewport_width * move_factor;
                field.RE_to -= viewport_width * move_factor;
            }
            if (kDown & KEY_UP) {
                field.IM_from -= viewport_height * move_factor;
                field.IM_to -= viewport_height * move_factor;
            }
            if (kDown & KEY_DOWN) {
                field.IM_from += viewport_height * move_factor;
                field.IM_to += viewport_height * move_factor;
            }
            if (kDown & KEY_R) {
                double new_width = viewport_width/zoom_factor;
                double dif_width = (viewport_width-new_width)/2;
                field.RE_from += dif_width;
                field.RE_to -= dif_width;

                double new_height = viewport_height/zoom_factor;
                double dif_height = (viewport_height-new_height)/2;
                field.IM_from += dif_height;
                field.IM_to -= dif_height;
            }

            if (kDown & KEY_L) {
                double new_width = viewport_width*zoom_factor;
                double dif_width = (viewport_width-new_width)/2;
                field.RE_from += dif_width;
                field.RE_to -= dif_width;

                double new_height = viewport_height*zoom_factor;
                double dif_height = (viewport_height-new_height)/2;
                field.IM_from += dif_height;
                field.IM_to -= dif_height;
            }
            if (kDown & KEY_X) {
                field = def;
            }

            run_mandel(field);
        }




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
