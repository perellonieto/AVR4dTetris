/*************************************************

        4Display Shield Library Example
        Code: Oscar Gonzalez December 2010
        www.BricoGeek.com
  
	Copyright(c) December 2010 Oscar Gonzalez - www.BricoGeek.com

	http://code.google.com/p/displayshield4d/

	Licensed under GNU General Public License v3 
        http://creativecommons.org/licenses/by/3.0/
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

 *************************************************/
#include "displayshield4d.h"
 
DisplayShield4d  oled;

const int max_size = 500;    // the max size of the snake
const int UpButton = 6;     // the number of the pushbutton pin
const int DownButton =  2;      // the number of the LED pin
const int LeftButton = 3;     // the number of the pushbutton pin
const int RightButton =  4;      // the number of the LED pin
const int PushButton = 5;     // the number of the pushbutton pin
const int CONTRAST = 2;    // 0 - 15
const unsigned int dark = 0x7BEF; // binary R = 01111 G = 011111 B = 01111

int led = 13; // led in the board

const int pixel = 6;

char author[] = "Miquel Perello Nieto";

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
position pos;
position ant;
position ini;
unsigned int color;

ISR(TIMER1_OVF_vect) 
{
  //TCNT1=0x0BDC;
  TCNT1=0xFBDC;
  second++;
  digitalWrite(led, second%2);
};

boolean toggle0 = 0;
ISR(TIMER0_COMPA_vect){//timer0 interrupt 2kHz toggles pin 8
//generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle0){
    toggle0 = 0;
  }
  else{
    toggle0 = 1;
  }
  oled.rectangle(30, 30, 100, 30, OLED_SOLID, oled.RGB(100, 200*toggle0, 100));
}

void(* reset) (void) = 0;


int tam_fig_h(int fig, int rot)
{
  switch(fig)
  {
    case 0:
      return 2;
    case 1:
      return rot%2?2:3;
    case 2:
      return rot%2?2:3;
    case 3:
      return rot%2?2:3;
    case 4:
      return rot%2?1:4;
    case 5:
      return rot%2?2:3;
    case 6:
      return rot%2?2:3;
  }
  return -1;
}

int tam_fig_v(int fig, int rot)
{
  switch(fig)
  {
    case 0:
      return 2;
    case 1:
      return rot%2?3:2;
    case 2:
      return rot%2?3:2;
    case 3:
      return rot%2?3:2;
    case 4:
      return rot%2?4:1;
    case 5:
      return rot%2?3:2;
    case 6:
      return rot%2?3:2;
  }
  return -1;
}

void paint_points()
{
  
        debug[0] = '0'+lines/1000;
        debug[1] = '0'+(lines%1000)/100;
        debug[2] = '0'+((lines%1000)%100)/10;
        debug[3] = '0'+((lines%1000)%100)%10;
        debug[4] = '\0';
        
        oled.rectangle(90, 50, 60, 25, OLED_SOLID, oled.RGB(100, 100, 100));
        oled.rectangle(90, 50, 60, 25, OLED_WIREFRAME, oled.RGB(200, 200, 200));
        oled.drawstringblock(95, 55, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 1, 1, "lineas");
        oled.drawstringblock(95, 65, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 1, 1, debug);
}

int paint_figure(int v, int h, int figure, int rot, unsigned int color)
{
  h = ini.h+(h*pixel);
  v = ini.v+(v*pixel);
  
  for (i = 0; i < 4; i++)
  {
     oled.rectangle(h+scheme[figure][rot][i].h*pixel, v+scheme[figure][rot][i].v*pixel, pixel-1, pixel-1, OLED_SOLID, color & dark);
     oled.rectangle(h+scheme[figure][rot][i].h*pixel+1, v+scheme[figure][rot][i].v*pixel+1, pixel-3, pixel-3, OLED_SOLID, color);
  }
}

int erase_figure(int v, int h, int figure, int rot)
{
  h = ini.h+h*pixel;
  v = ini.v+v*pixel;
  unsigned int color_erase = oled.RGB(0,0,0);
  
  for (i = 0; i < 4; i++)
  {
     oled.rectangle(h+scheme[figure][rot][i].h*pixel, v+scheme[figure][rot][i].v*pixel, pixel-1, pixel-1, OLED_SOLID, color_erase);
  }
}

void show_menu()
{
  
  oled.setfont(OLED_FONT5x7);
  oled.setfontmode(OLED_FONT_TRANSPARENT);
  
  /* Barrido de pantalla inicial */
  for (i=0 ; i<128 ; i++) { oled.line(0, i, 159, i, oled.RGB(255-i*2, 0, 127+i)); }
  for (i=0 ; i<128 ; i++) { oled.line(0, 127-i, 159, 127-i, oled.RGB(i*2, 255-i*2, 127+i)); }
  //delay(100);
  for (i=0 ; i<160 ; i++) { oled.line(159-i, 0, 159-i, 127, oled.RGB(255-i, i, 0)); }
  //delay(100);
  for (i=0 ; i<160 ; i++) { oled.line(i, 0, i, 127, oled.RGB(0, 255-i, 255-i)); }
  //delay(100);
  for (i=0 ; i<255 ; i++) { oled.circle(80, 64, i/3, OLED_SOLID, oled.RGB(i, i, i)); }
  for (i=0 ; i<255 ; i++) { oled.circle(80, 64, (255-i)/2, OLED_SOLID, oled.RGB(i, i, 0)); }
  //oled.Clear();
  
  for (i=0 ; i<128 ; i++) { oled.line(0, i, 159, i, oled.RGB(255-i*2, 0, 127+i)); }
  oled.Clear();
  oled.rectangle(30, 30, 100, 30, OLED_SOLID, oled.RGB(100, 200, 100));
  delay(1000);

  
  //oled.drawstringblock(5, 5, 0, oled.RGB(0,0,0), 10, 10, author);
  //delay(1000);
  //oled.drawstringblock(5, 40, 0, oled.RGB(0, 255, 0), 1, 3, author);
  //delay(1000);


  //oled.drawstringblock(5, 5, 0, oled.RGB(255, 255, 255), 2, 2, "Arduino 2560");
  //oled.drawstringblock(5, 40, 0, oled.RGB(0, 255, 0), 1, 3, "Miquel Perello Nieto");
  //oled.drawstringblock(5, 100, 0, oled.RGB(255, 0, 255), 1, 1, "www.mk137.dyndns.org");
  delay(1000); 
  oled.Clear(); 
  
  
  oled.rectangle(30, 30, 100, 30, OLED_SOLID, oled.RGB(100, 200, 100));
  delay(2000);
  //oled.drawstringblock(35, 25, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 2, 2, "TETRIS");
  //foled.drawstringblock(60, 105, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 1, 1, "Start");
  
  while (digitalRead(PushButton) == HIGH)
  {
    if (second%30 < 15) oled.drawstringblock(60, 105, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 1, 1, "Start");
    else oled.drawstringblock(60, 105, OLED_FONT_OPAQUE, oled.RGB(0, 0, 0), 1, 1, "Start");
  }

  //delay(250);
  oled.Clear();
 
}

int limit(int v, int h, int fig, int rot)
{
  if (v > 20-tam_fig_v(fig,rot))
    return 1;
  
  for (i = 0; i < 4; i++)
  {
    if (board[v+scheme[fig][rot][i].v][h+scheme[fig][rot][i].h] != -1) return 1;
  }
  
  return 0;
}

void write_board_figure(int v, int h, int fig, int rot, unsigned int color)
{
  
  for (i = 0; i < 4; i++)
  {
    board[v+scheme[fig][rot][i].v][h+scheme[fig][rot][i].h] = color;
  }
  
}

void erase_board_line(int v)
{
  int i;
  int j;
  
  oled.rectangle(ini.h, ini.v+v*pixel, pixel*10-1, pixel-1, OLED_SOLID, oled.RGB(0,0,0));
  
  for (i = v; i > 1; i--)
  {
    for (j = 0; j < 10; j++)
    {
      board[i][j] = board[i-1][j];
      if (board[i][j] != -1)
      {
        oled.rectangle(ini.h+j*pixel, ini.v+i*pixel, pixel-1, pixel-1, OLED_SOLID, board[i][j] & dark);
        oled.rectangle(ini.h+j*pixel+1, ini.v+i*pixel+1, pixel-3, pixel-3, OLED_SOLID, board[i][j]);
      }
      else
        oled.rectangle(ini.h+j*pixel, ini.v+i*pixel, pixel-1, pixel-1, OLED_SOLID, oled.RGB(0,0,0));
    }
  }
  
  
  for (j = 0; j < 10; j++)
  {
    board[0][j] = -1; 
  }
  
  oled.rectangle(ini.h, ini.v, pixel*10-1, pixel-1, OLED_SOLID, oled.RGB(0,0,0));
}

void test_and_erase()
{
  int i;
  int j;
  int borrar;
  
  for (i = 19; i > 0; i--)
  {
    borrar = 1;
    
    while (borrar == 1)
    {
      /* DEBUG
      oled.rectangle(ini.h-6, ini.v+pixel*i, pixel-1, pixel-1, OLED_SOLID, oled.RGB(i*10,0,i*10));
      while (digitalRead(PushButton) != LOW);
      delay(50);
      END DEBUG*/
      
      for (j = 0; borrar == 1 && j < 10; j++)
      {
        if (board[i][j] == -1) borrar = 0;
      }
      
      if (borrar == 1) 
      {
        
        /*DEBUG
              debug[0] = '0'+i/10;
              debug[1] = '0'+i%10;
              debug[1] = '\0';
              
              oled.rectangle(90, 80, 60, 25, OLED_SOLID, oled.RGB(100, 100, 100));
              oled.rectangle(90, 80, 60, 25, OLED_WIREFRAME, oled.RGB(200, 200, 200));
              oled.drawstringblock(95, 85, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 1, 1, "delete");
              oled.drawstringblock(95, 95, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 1, 1, debug);
        END DEBUG*/
        
        erase_board_line(i);
        lines++;
        paint_points();
      }
    }
  }
      /* DEBUG
      while (digitalRead(PushButton) != LOW);
      delay(50);
      oled.rectangle(ini.h-6, ini.v+pixel*i, pixel-1, pixel*20, OLED_SOLID, oled.RGB(200, 100, 100));
      END DEBUG */
}

int can_put(int v, int h, int fig, int rot)
{
  int i;
  for (i = 0; i < 4; i++)
  {
    if (board[v+scheme[fig][rot][i].v][h+scheme[fig][rot][i].h] != -1) return 0;
  }
  return 1;
}

void setup()
{
  Serial.begin(57600);  
  oled.Init();
  oled.SetContrast(CONTRAST);
  oled.Clear();
  
  randomSeed(analogRead(0));
  
  /* Joistick */
  pinMode(UpButton, INPUT);
  digitalWrite(UpButton, HIGH);
  pinMode(DownButton, INPUT);
  digitalWrite(DownButton, HIGH);
  pinMode(LeftButton, INPUT);
  digitalWrite(LeftButton, HIGH);
  pinMode(RightButton, INPUT);
  digitalWrite(RightButton, HIGH);
  pinMode(PushButton, INPUT);
  digitalWrite(PushButton, HIGH);
  
    
  TIMSK1=0x01;    // enabled global and timer overflow interrupt;
  TCCR1A = 0x00;  // normal operation page 148 (mode0);
  TCNT1=0x0BDC;   // 16bit counter register
  TCCR1B = 0x04;  // start timer/ set clock
  
  // End of test timer0
  
  show_menu();
  
  lines = 0;
  ini.h = 20;
  ini.v = 2;
  
  fig = random(5)+1;
  pos.v = 0;
  pos.h = 3;
  time = 500;
  wait = 0;
  
  change_color();

   for (i = 0; i < 20; i++)
   {
    for (j = 0; j < 10; j++)
    {
      board[i][j] = -1;
      //oled.rectangle(ini.h+j*pixel, ini.v+i*pixel, pixel-1, pixel-1, OLED_WIREFRAME, oled.RGB(100,100,100));
    }
   }
 
  oled.SetBackground(oled.RGB(200, 100, 100));
  oled.rectangle(ini.h-1, ini.v-1, pixel*10+1, pixel*20+1, OLED_SOLID, oled.RGB(0,0,0));
  oled.rectangle(ini.h-1, ini.v-1, pixel*10+1, pixel*20+1, OLED_WIREFRAME, oled.RGB(255,255,255));
  paint_points();
  oled.drawstringblock(90, 25, OLED_FONT_OPAQUE, oled.RGB(255, 255, 150), 1, 2, "TETRIS");
}

void change_color()
{
     switch (random(6))
    {
      case 0:
        color = oled.RGB(255,0,0);
        break;
      case 1:
        color = oled.RGB(0,255,0);
        break;
      case 2:
        color = oled.RGB(0,0,255);
        break;
      case 3:
        color = oled.RGB(255,255,0);
        break;
      case 4:
        color = oled.RGB(0,255,255);
        break;
      case 5:
        color = oled.RGB(255,0,255);
        break;
    } 
}

void game_over()
{
        debug[0] = '0'+lines/1000;
        debug[1] = '0'+(lines%1000)/100;
        debug[2] = '0'+((lines%1000)%100)/10;
        debug[3] = '0'+((lines%1000)%100)%10;
        debug[4] = '\0';
        
        oled.rectangle(84, 50, 70, 62, OLED_SOLID, oled.RGB(0, 0, 0));
        oled.rectangle(84, 50, 70, 62, OLED_WIREFRAME, oled.RGB(255, 0, 0));
        oled.drawstringblock(88, 55, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 2, 2, "GAME");
        oled.drawstringblock(88, 75, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 2, 2, "OVER");
        oled.drawstringblock(88, 95, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 2, 2, debug);
        
       for (i = 0; i < 20; i++)
       {
        for (j = 0; j < 10; j++)
        {
          board[i][j] = -1;
        }
       }
        
        while (digitalRead(PushButton) == HIGH);
        
        lines = 0;
        oled.rectangle(84, 50, 70, 62, OLED_SOLID, oled.RGB(200, 100, 100));
        oled.rectangle(ini.h-1, ini.v-1, pixel*10+1, pixel*20+1, OLED_SOLID, oled.RGB(0,0,0));
        oled.rectangle(ini.h-1, ini.v-1, pixel*10+1, pixel*20+1, OLED_WIREFRAME, oled.RGB(255,255,255));
        paint_points();
        oled.drawstringblock(90, 25, OLED_FONT_OPAQUE, oled.RGB(255, 255, 150), 1, 2, "TETRIS");
        
}
void loop()
{
  
  /* ================================ *
   * Calculando color para mostrar    *
   * ================================ */
  //if (pos.v > 20-tam_fig_v(fig))
  if (limit(pos.v, pos.h, fig, rot) == 1)
  {
    write_board_figure(ant.v, ant.h, fig, rot, color);
    
    test_and_erase();
    
    change_color();

    fig = random(7);
    rot = random(4);
    pos.v = 0;
    pos.h = 3;
    time = 500;
    wait = second;
    
    if (limit(pos.v, pos.h, fig, rot) == 1)
    {
      game_over();
    }
  }
  else
  {
    if (paint == 1) erase_figure(ant.v, ant.h, fig, ant_rot);
  }
  
    ant.h = pos.h;
    ant.v = pos.v;
    ant_rot = rot;
  
  if (paint == 1) 
  {
    paint_figure(pos.v, pos.h, fig, rot, color);
    paint = 0;
  }
  
     /* Llegint el joistick */
     if ((second - wait) > 10)
     {
      if (digitalRead(UpButton) == LOW || digitalRead(PushButton) == LOW)
      { 
        aux = (rot+1)%4;
        if (pos.h < 11-tam_fig_h(fig, aux) && can_put(pos.v, pos.h, fig, aux) == 1)
        {
          rot = aux;
          paint = 1;
          wait = second;
        }
      }
      if (digitalRead(DownButton) == LOW) 
      { 
        //pos.v++;
        //paint = 1;
        wait = second;
        second += 15;
      }
      if (digitalRead(LeftButton) == LOW) 
      { 
        if (pos.h > 0 && can_put(pos.v, pos.h-1, fig, rot) == 1) 
        {
          pos.h--;
          paint = 1;
          wait = second;
        }
      }
      if (digitalRead(RightButton) == LOW) 
      { 
        if (pos.h < 10-tam_fig_h(fig, rot) && can_put(pos.v, pos.h+1, fig, rot) == 1)
        {
          pos.h++;
          paint = 1;
          wait = second;
        }
      }
    }
    
  if (second - ant_seconds > 50-lines/10)
  {
    ant_seconds = second;
    pos.v++;
    paint = 1;
  }
  
}
