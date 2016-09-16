#include <3ds.h>
#include <string>
#include <cstdio>
#include "mandel.h"
#include <unistd.h>
#include "metrics.h"
#include "string_utils.h"

using namespace std;


PrintConsole bottom, top;
size_t quantify(size_t input, size_t min, size_t max, size_t pallet_size){
    // returns from 0 to pallet_size-1 inclusive
    size_t range = max-min;
    double quant_range = double(range)/pallet_size;
    if (input <= min) return 0;
    if (input >= max) return pallet_size-1;
    size_t current = (size_t )(double(input)/quant_range);

    if(current >= pallet_size){
        printf("\nerror, %d->%d, in(%d:%d), pallet: %d\n", input, current, min, max, pallet_size);

    }
    return current;
}

void run_pallet(vector<bgr_pixel> pallet){
    gfxSetDoubleBuffering(GFX_TOP, false);
    u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
    for (size_t i = 0; i < TOP_PIXELS; ++i) {
        size_t quant = quantify(i, 0, TOP_PIXELS, pallet.size());
        bgr_pixel px = pallet[quant];

        fb[i*3] = px.b;
        fb[i*3+1] = px.g;
        fb[i*3+2] = px.r;
    }
}

void run_mandel_gfx(FieldDef field, RenderOptions options){

    gfxSetDoubleBuffering(GFX_TOP, false);

    // getting pointer to lower screen frame buffer
    u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
    vector<vector<size_t>> iters = getField(field, options);

    histogram_acc h;
    map<size_t, size_t> mapping;
    size_t quant;
    if(options.useHistogram){
        h = histogram(iters);
        mapping = buildPalletMappings(h, options.pallet.size());
    }
    for (size_t i = 0; i < iters.size(); ++i) {
        vector<size_t> line = iters[i];

        for (size_t j = 0; j < line.size(); ++j) {
            size_t cell = line.at(j);

            if(options.useHistogram){
                quant = mapping[cell];
            } else {
                quant = quantify(cell, 0, options.max_iter, options.pallet.size());
            }
            if(quant >= options.pallet.size()){
                printf("FAIL %d", quant);

            }
            size_t offset = ((iters.size()-i-1)+j*iters.size())*3;
            bgr_pixel px = options.pallet[quant];
            fb[offset] = px.b;
            fb[offset+1] = px.g;
            fb[offset+2] = px.r;
        }

    }
}

void run_mandel(FieldDef field, RenderOptions options){
    consoleInit(GFX_TOP, NULL);
    vector<string> pallet = preparePallet(true);

    vector<vector<size_t>> iters = getField(field, options );

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
            printf(pallet[quant].c_str());
            gfxFlushBuffers();

        }

    }

}

void run_print_stats(FieldDef def, RenderOptions options) {
    consoleInit(GFX_BOTTOM, &bottom);
    consoleSelect(&bottom);
    printf(colorize(COLOR_MAGENTA, "Running mandelGFX\n").c_str());
    printf("\n");
    printf("%s: (% .4e, % .4e)\n", colorize(COLOR_WHITE, COLOR_BLUE, "IM").c_str(), def.IM_from, def.IM_to);
    printf("%s: (% .4e, % .4e)\n", colorize(COLOR_WHITE, COLOR_RED, "RE").c_str(), def.RE_from, def.RE_to);
    printf("Display %dx%d\n", def.nRE, def.nIM);
    printf("Pallet size %d\n", options.pallet.size());
    printf("[A] Histogram color correction: [%s]\n", options.useHistogram?
                                  colorize(COLOR_WHITE,COLOR_GREEN,"YES").c_str():
                                  colorize(COLOR_WHITE,COLOR_RED,"NO").c_str());
    printf("[Y] Max iterations: %d\n", options.max_iter);

}

void fix_aspect(FieldDef* def, double new_aspect){
    double def_h = def->IM_to - def->IM_from;
    double def_w = def->RE_to - def->RE_from;
    double new_w = def_h* new_aspect;
    double d_w = (new_w-def_w)/2.0;
    def->RE_from-=d_w;
    def->RE_to+=d_w;
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

    FieldDef def =  {
            TOP_WIDTH,
            TOP_HEIGHT,
            -2.25,
            -2.25+default_width,
            -1.5,
            -1.5+default_height
    };
    double aspect = (double)TOP_WIDTH/(double)TOP_HEIGHT;
    fix_aspect(&def, aspect);
    FieldDef field =def;
    double move_factor = 0.25; // of viewport, not absolute
    double zoom_factor = 1.5; // of viewport, not absolute.
    size_t Q_DRAFT_MAX_ITER = 50;
    size_t Q_BEST_MAX_ITER = 500;
    bool useHistogram = true;

    RenderOptions options ={
            Q_DRAFT_MAX_ITER,
            true,
            prepareGFXPallet(16),
            2.0f
    };
    run_pallet(options.pallet);
    printf(colorize(COLOR_RED, "Now starting...\n").c_str());
    usleep(1000000);
    run_print_stats(field, options);
    run_mandel_gfx(field, options);
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
                options.max_iter = (options.max_iter == Q_DRAFT_MAX_ITER? Q_BEST_MAX_ITER:Q_DRAFT_MAX_ITER);
            }
            if (kDown & KEY_A) {
                useHistogram = !useHistogram;
            }

            run_print_stats(field, options);
            run_mandel_gfx(field, options);
            printf(colorize(COLOR_CYAN, "Ready").c_str());
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
