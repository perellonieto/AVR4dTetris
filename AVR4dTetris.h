#ifndef _AVR4DTETRIS_H_
#define _AVR4DTETRIS_H_

const int max_size = 500;    // the max size of the snake
const int UpButton = 6;     // the number of the pushbutton pin
const int DownButton =  2;      // the number of the LED pin
const int LeftButton = 3;     // the number of the pushbutton pin
const int RightButton =  4;      // the number of the LED pin
const int PushButton = 5;     // the number of the pushbutton pin
const int CONTRAST = 2;    // 0 - 15
const unsigned int dark = 0x7BEF; // binary R = 01111 G = 01111 B = 01111

const int pixel = 6;

struct position {
  int v;
  int h;
};


int board[20][10];
char puntos[4];
char debug[12];
int i;
int j;
int aux;
int fig;
int rot;
int ant_rot;
int time;
unsigned int second;
unsigned int ant_seconds;
int paint;
int lines;
int wait;
position p;
position scheme[7][4][4] = {
                            // ##                         ##                          ##                          ##
                            // ##                         ##                          ##                          ##
                            {{{0,0},{0,1},{1,0},{1,1}} , {{0,0},{0,1},{1,0},{1,1}} , {{0,0},{0,1},{1,0},{1,1}} , {{0,0},{0,1},{1,0},{1,1}}},
                            //  #                          #                          ###                         #
                            // ###                        ##                           #                          ##
                            //                             #                                                      #
                            {{{0,1},{1,0},{1,1},{1,2}} , {{0,1},{1,0},{1,1},{2,1}} , {{0,0},{0,1},{0,2},{1,1}} , {{0,0},{1,0},{1,1},{2,0}}},
                            //  ##                        #                            ##                         #
                            // ##                         ##                          ##                          ##
                            //                             #                                                       #
                            {{{0,1},{0,2},{1,0},{1,1}} , {{0,0},{1,0},{1,1},{2,1}} , {{0,1},{0,2},{1,0},{1,1}} , {{0,0},{1,0},{1,1},{2,1}}},
                            // ##                          #                          ##                           #
                            //  ##                        ##                           ##                         ##
                            //                            #                                                       #
                            {{{0,0},{0,1},{1,1},{1,2}} , {{0,1},{1,0},{1,1},{2,0}} , {{0,0},{0,1},{1,1},{1,2}} , {{0,1},{1,0},{1,1},{2,0}}},
                            // ####                       #                           ####                        #
                            //                            #                                                       #
                            //                            #                                                       #
                            //                            #                                                       #
                            {{{0,0},{0,1},{0,2},{0,3}} , {{0,0},{1,0},{2,0},{3,0}} , {{0,0},{0,1},{0,2},{0,3}} , {{0,0},{1,0},{2,0},{3,0}}},
                            //   #                        ##                          ###                        #
                            // ###                         #                          #                          #
                            //                             #                                                     ##
                            {{{0,2},{1,0},{1,1},{1,2}} , {{0,0},{0,1},{1,1},{2,1}} , {{0,0},{0,1},{0,2},{1,0}} , {{0,0},{1,0},{2,0},{2,1}}},
                            // ###                        ##                          #                            #
                            //   #                        #                           ###                          #
                            //                            #                                                       ##
                            {{{0,0},{0,1},{0,2},{1,2}} , {{0,0},{0,1},{1,0},{2,0}} , {{0,0},{1,0},{1,1},{1,2}} , {{0,1},{1,1},{2,0},{2,1}}}
                            };

DisplayShield4d  oled;
const unsigned int N_FIG_COLORS = 6;
const unsigned int figure_colors[N_FIG_COLORS] = {
        oled.RGB(255,0,0),
        oled.RGB(0,255,0),
        oled.RGB(0,0,255),
        oled.RGB(255,255,0),
        oled.RGB(0,255,255),
        oled.RGB(255,0,255)
};

position pos;
position ant;
position ini;
unsigned int color;

#endif
