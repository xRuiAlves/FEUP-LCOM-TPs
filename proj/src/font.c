#include "font.h"
#include "video_gr.h"

// The small digits bitmaps
static Bitmap* bmp0;
static Bitmap* bmp1;
static Bitmap* bmp2;
static Bitmap* bmp3;
static Bitmap* bmp4;
static Bitmap* bmp5;
static Bitmap* bmp6;
static Bitmap* bmp7;
static Bitmap* bmp8;
static Bitmap* bmp9;
static Bitmap* bar;
static Bitmap* space;
static Bitmap* twoDots;

// The big digits bitmaps
static Bitmap* bmpBig0;
static Bitmap* bmpBig1;
static Bitmap* bmpBig2;
static Bitmap* bmpBig3;
static Bitmap* bmpBig4;
static Bitmap* bmpBig5;
static Bitmap* bmpBig6;
static Bitmap* bmpBig7;
static Bitmap* bmpBig8;
static Bitmap* bmpBig9;


void initialize_font(){
    bmp0 = loadBitmap(BITMAP_0_PATH);
    bmp1 = loadBitmap(BITMAP_1_PATH);
    bmp2 = loadBitmap(BITMAP_2_PATH);
    bmp3 = loadBitmap(BITMAP_3_PATH);
    bmp4 = loadBitmap(BITMAP_4_PATH);
    bmp5 = loadBitmap(BITMAP_5_PATH);
    bmp6 = loadBitmap(BITMAP_6_PATH);
    bmp7 = loadBitmap(BITMAP_7_PATH);
    bmp8 = loadBitmap(BITMAP_8_PATH);
    bmp9 = loadBitmap(BITMAP_9_PATH);
    bar =  loadBitmap(BITMAP_BAR_PATH);
    space = loadBitmap(BITMAP_SPACE_PATH);
    twoDots = loadBitmap(BITMAP_TWO_DOTS_PATH);

    bmpBig0 = loadBitmap(BITMAP_BIG_0_PATH);
    bmpBig1 = loadBitmap(BITMAP_BIG_1_PATH);
    bmpBig2 = loadBitmap(BITMAP_BIG_2_PATH);
    bmpBig3 = loadBitmap(BITMAP_BIG_3_PATH);
    bmpBig4 = loadBitmap(BITMAP_BIG_4_PATH);
    bmpBig5 = loadBitmap(BITMAP_BIG_5_PATH);
    bmpBig6 = loadBitmap(BITMAP_BIG_6_PATH);
    bmpBig7 = loadBitmap(BITMAP_BIG_7_PATH);
    bmpBig8 = loadBitmap(BITMAP_BIG_8_PATH);
    bmpBig9 = loadBitmap(BITMAP_BIG_9_PATH);
}

void terminate_font(){
    deleteBitmap(bmp0);
    deleteBitmap(bmp1);
    deleteBitmap(bmp2);
    deleteBitmap(bmp3);
    deleteBitmap(bmp4);
    deleteBitmap(bmp5);
    deleteBitmap(bmp6);
    deleteBitmap(bmp7);
    deleteBitmap(bmp8);
    deleteBitmap(bmp9);
    deleteBitmap(bar);
    deleteBitmap(space);
    deleteBitmap(twoDots);

    deleteBitmap(bmpBig0);
    deleteBitmap(bmpBig1);
    deleteBitmap(bmpBig2);
    deleteBitmap(bmpBig3);
    deleteBitmap(bmpBig4);
    deleteBitmap(bmpBig5);
    deleteBitmap(bmpBig6);
    deleteBitmap(bmpBig7);
    deleteBitmap(bmpBig8);
    deleteBitmap(bmpBig9);
}

void draw_string(char string[] , int string_size , short x_pos , short y_pos , u_char sizeOfFont , char* buffer){
    int i = 0;

    if(sizeOfFont == SMALL){
        for (i=0 ; i<string_size ; i++){
            draw_digit(string[i] , x_pos , y_pos , SMALL , buffer);
            x_pos += DIGIT_BMP_SIZE + DIGIT_SEPARATION_SIZE;
        }
    }

    else{
        for (i=0 ; i<string_size ; i++){
            draw_digit(string[i] , x_pos , y_pos , BIG , buffer);
            x_pos += BIG_DIGIT_SIZE + BIG_DIGIT_SEPARATION_SIZE;
        }
    }
}

void draw_digit(char digit_to_draw , short x_pos , short y_pos, u_char sizeOfDigit , char* buffer){

    if(sizeOfDigit == SMALL){
        switch(digit_to_draw){
        case '0':
            drawBitmapWithTransparency(bmp0 , x_pos , y_pos , buffer);
            break;
        case '1':
            drawBitmapWithTransparency(bmp1 , x_pos , y_pos , buffer);
            break;
        case '2':
            drawBitmapWithTransparency(bmp2 , x_pos , y_pos , buffer);
            break;
        case '3':
            drawBitmapWithTransparency(bmp3 , x_pos , y_pos , buffer);
            break;
        case '4':
            drawBitmapWithTransparency(bmp4 , x_pos , y_pos , buffer);
            break;
        case '5':
            drawBitmapWithTransparency(bmp5 , x_pos , y_pos , buffer);
            break;
        case '6':
            drawBitmapWithTransparency(bmp6 , x_pos , y_pos , buffer);
            break;
        case '7':
            drawBitmapWithTransparency(bmp7 , x_pos , y_pos , buffer);
            break;
        case '8':
            drawBitmapWithTransparency(bmp8 , x_pos , y_pos , buffer);
            break;
        case '9':
            drawBitmapWithTransparency(bmp9 , x_pos , y_pos , buffer);
            break;
        case '/':
            drawBitmapWithTransparency(bar , x_pos , y_pos , buffer);
            break;
        case ' ':
            drawBitmapWithTransparency(space , x_pos , y_pos , buffer);
            break;
        case ':':
            drawBitmapWithTransparency(twoDots , x_pos , y_pos , buffer);
            break;
        }
    }

    else{
        switch(digit_to_draw){
        case '0':
            drawBitmapWithTransparency(bmpBig0 , x_pos , y_pos , buffer);
            break;
        case '1':
            drawBitmapWithTransparency(bmpBig1 , x_pos , y_pos , buffer);
            break;
        case '2':
            drawBitmapWithTransparency(bmpBig2 , x_pos , y_pos , buffer);
            break;
        case '3':
            drawBitmapWithTransparency(bmpBig3 , x_pos , y_pos , buffer);
            break;
        case '4':
            drawBitmapWithTransparency(bmpBig4 , x_pos , y_pos , buffer);
            break;
        case '5':
            drawBitmapWithTransparency(bmpBig5 , x_pos , y_pos , buffer);
            break;
        case '6':
            drawBitmapWithTransparency(bmpBig6 , x_pos , y_pos , buffer);
            break;
        case '7':
            drawBitmapWithTransparency(bmpBig7 , x_pos , y_pos , buffer);
            break;
        case '8':
            drawBitmapWithTransparency(bmpBig8 , x_pos , y_pos , buffer);
            break;
        case '9':
            drawBitmapWithTransparency(bmpBig9 , x_pos , y_pos , buffer);
            break;
        }
    }

}
