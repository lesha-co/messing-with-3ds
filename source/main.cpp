#include <3ds.h>
#include <string>
#include <cstdio>
#include "mandel.h"
#include <unistd.h>
#include "metrics.h"
using namespace std;


PrintConsole bottom, top;

void run_mandel_gfx(fieldDef field, size_t max_iter){

    gfxSetDoubleBuffering(GFX_TOP, false);
    double tolerance = 2.0;
    vector<bgr_pixel> pallet = prepareGFXPallet(8);
    // getting pointer to lower screen frame buffer
    u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
    vector<vector<size_t>> iters = getField(field, max_iter, tolerance);
    histogram_acc h = histogram(iters);
    map<size_t, size_t> mapping = buildPalletMappings(h, pallet.size());

    for (size_t i = 0; i < iters.size(); ++i) {
        vector<size_t> line = iters[i];

        for (size_t j = 0; j < line.size(); ++j) {
            size_t cell = line.at(j);
            size_t quant = mapping[cell];
            if(quant >= pallet.size()){
                printf("FAIL %d", quant);

            }
            size_t offset = (i+j*iters.size())*3;
            bgr_pixel px = pallet[quant];
            fb[offset] = px.b;
            fb[offset+1] = px.g;
            fb[offset+2] = px.r;
        }

    }
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
    consoleInit(GFX_BOTTOM, &bottom);
    consoleSelect(&bottom);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// run once begin
    ///
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

    size_t Q_DRAFT_MAX_ITER = 50;
    size_t Q_BEST_MAX_ITER = 500;
    size_t CURRENT_Q = Q_DRAFT_MAX_ITER;

    run_mandel_gfx(field, CURRENT_Q);
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
            if (kDown & KEY_Y) {
                CURRENT_Q = (CURRENT_Q == Q_DRAFT_MAX_ITER? Q_BEST_MAX_ITER:Q_DRAFT_MAX_ITER);
            }

            run_mandel_gfx(field, CURRENT_Q);
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
