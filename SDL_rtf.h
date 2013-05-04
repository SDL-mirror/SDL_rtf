/*
  SDL_rtf:  A companion library to SDL for displaying RTF format text
  Copyright (C) 2003-2012 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/* $Id$ */

#ifndef _SDL_RTF_H
#define _SDL_RTF_H

#include "SDL.h"
#include "begin_code.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Printable format: "%d.%d.%d", MAJOR, MINOR, PATCHLEVEL
*/
#define SDL_RTF_MAJOR_VERSION   2
#define SDL_RTF_MINOR_VERSION   0
#define SDL_RTF_PATCHLEVEL      0

/* This macro can be used to fill a version structure with the compile-time
 * version of the SDL_rtf library.
 */
#define SDL_RTF_VERSION(X)                                              \
{                                                                       \
        (X)->major = SDL_RTF_MAJOR_VERSION;                                \
        (X)->minor = SDL_RTF_MINOR_VERSION;                                \
        (X)->patch = SDL_RTF_PATCHLEVEL;                                \
}

/* Backwards compatibility */
#define RTF_MAJOR_VERSION       SDL_RTF_MAJOR_VERSION
#define RTF_MINOR_VERSION       SDL_RTF_MINOR_VERSION
#define RTF_PATCHLEVEL          SDL_RTF_PATCHLEVEL
#define RTF_VERSION(X)          SDL_RTF_VERSION(X)

/* This function gets the version of the dynamically linked SDL_rtf library.
   it should NOT be used to fill a version structure, instead you should
   use the SDL_RTF_VERSION() macro.
 */
extern DECLSPEC const SDL_version * SDLCALL RTF_Linked_Version(void);

typedef struct _RTF_Context RTF_Context;

typedef enum
{
    RTF_FontDefault,    /* Unknown or default font */
    RTF_FontRoman,      /* Proportionally spaced serif fonts,
                           e.g. Times New Roman, Palatino */
    RTF_FontSwiss,      /* Proportionally spaced sans serif fonts,
                           e.g. Arial */
    RTF_FontModern,     /* Fixed pitch serif and sans serif fonts,
                           e.g. Courier New, Pica */
    RTF_FontScript,     /* Script fonts, e.g. Cursive */
    RTF_FontDecor,      /* Decorative fonts, e.g. Zapf Chancery */
    RTF_FontTech,       /* Technical, symbol, and math fonts,
                           e.g. Symbol */
    RTF_FontBidi        /* Bidirectional fonts, like Arabic or Hebrew */
}
RTF_FontFamily;

typedef enum
{
    RTF_FontNormal    = 0x00,
    RTF_FontBold      = 0x01,
    RTF_FontItalic    = 0x02,
    RTF_FontUnderline = 0x04
}
RTF_FontStyle;

/* Various functions that need to be provided to give SDL_rtf font support */

#define RTF_FONT_ENGINE_VERSION 1

typedef struct _RTF_FontEngine
{
    int version;        /* This should be set to RTF_FONT_ENGINE_VERSION */

    /* A function to create a font matching the requested parameters.
       The family is one of those listed in the RTF_FontFamily enum.
       The charset is a Windows character set
       The size is in points.
       The style is a bitmask of the constants in RTF_FontStyle.
     */
    void *(*CreateFont)(const char *name, RTF_FontFamily family, int charset, int size, int style);

    /* Return the spacing in pixels between rows of text using this font */
    int (*GetLineSpacing)(void *font);

    /* Fill in and return the byte and pixel offsets to each character
     * within the given UTF-8 text.
     */
    int (*GetCharacterOffsets)(void *font, const char *text, int *byteOffsets, int *pixelOffsets, int maxOffsets);

    /* Create a texture containing a row of the given UTF-8 text */
    SDL_Texture *(*RenderText)(void *font, SDL_Renderer *renderer, const char *text, SDL_Color fg);

    /* Free a font */
    void (*FreeFont)(void *font);
} RTF_FontEngine;


/* Create an RTF display context, with the given font engine.
 * Once a context is created, it can be used to load and display
 * text in Microsoft RTF format.
 */
extern DECLSPEC RTF_Context * SDLCALL RTF_CreateContext(SDL_Renderer *renderer, RTF_FontEngine *fontEngine);

/* Set the text of an RTF context.
 * This function returns 0 if it succeeds or -1 if it fails.
 * Use RTF_GetError() to get a text message corresponding to the error.
 * This can be called multiple times to change the text displayed.
 */
extern DECLSPEC int SDLCALL RTF_Load(RTF_Context *ctx, const char *file);
extern DECLSPEC int SDLCALL RTF_Load_RW(RTF_Context *ctx, SDL_RWops *src, int freesrc);

/* Get the title of an RTF document */
extern DECLSPEC const char * SDLCALL RTF_GetTitle(RTF_Context *ctx);

/* Get the subject of an RTF document */
extern DECLSPEC const char * SDLCALL RTF_GetSubject(RTF_Context *ctx);

/* Get the author of an RTF document */
extern DECLSPEC const char * SDLCALL RTF_GetAuthor(RTF_Context *ctx);

/* Get the height of an RTF render area given a certain width.
 * The text is automatically reflowed to this new width, and should match
 * the width of the clipping rectangle used for rendering later.
 */
extern DECLSPEC int SDLCALL RTF_GetHeight(RTF_Context *ctx, int width);

/* Render the RTF document to a rectangle of a surface.
   The text is reflowed to match the width of the rectangle.
   The rendering is offset up (and clipped) by yOffset pixels.
*/
extern DECLSPEC void SDLCALL RTF_Render(RTF_Context *ctx, SDL_Rect *rect, int yOffset);
 
/* Free an RTF display context */
extern DECLSPEC void SDLCALL RTF_FreeContext(RTF_Context *ctx);
 
/* We'll use SDL for reporting errors */
#define RTF_SetError    SDL_SetError
#define RTF_GetError    SDL_GetError

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* _SDL_RTF_H */

