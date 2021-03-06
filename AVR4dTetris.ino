/*************************************************

        4Display Shield Tetris
        Code: Miquel Perello Nieto
        
        This code is an adaptation of the example made by Oscar Gonzalez:
        
        4Display Shield Library Example
        Code: Oscar Gonzalez December 2010
        www.BricoGeek.com
  
        Copyright(c) January 2017 Miquel Perello Nieto - www.perellonieto.com
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
#include "AVR4dTetris.h"

ISR(TIMER1_OVF_vect) {
  //TCNT1=0x0BDC;
  TCNT1=0xFBDC;
  second++;
};

void(* reset) (void) = 0;

// Figure width with the given rotation
int fig_width(int fig, int rot){
  switch(fig){
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

// Figure height with the given rotation
int fig_height(int fig, int rot){
  switch(fig){
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

void paint_points(){
        debug[0] = '0'+lines/1000;
        debug[1] = '0'+(lines%1000)/100;
        debug[2] = '0'+((lines%1000)%100)/10;
        debug[3] = '0'+((lines%1000)%100)%10;
        debug[4] = '\0';
        
        oled.rectangle(90, 50, 60, 25, OLED_SOLID, oled.RGB(100, 100, 100));
        oled.rectangle(90, 50, 60, 25, OLED_WIREFRAME, oled.RGB(200, 200, 200));
        oled.drawstringblock(95, 55, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 1, 1, "score");
        oled.drawstringblock(95, 65, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 1, 1, debug);
}

int paint_figure(int v, int h, int figure, int rot, unsigned int color){
  h = ini.h+(h*pixel);
  v = ini.v+(v*pixel);
  
  for (int i = 0; i < 4; i++){
     oled.rectangle(h+scheme[figure][rot][i].h*pixel, v+scheme[figure][rot][i].v*pixel, pixel-1, pixel-1, OLED_SOLID, color & dark);
     oled.rectangle(h+scheme[figure][rot][i].h*pixel+1, v+scheme[figure][rot][i].v*pixel+1, pixel-3, pixel-3, OLED_SOLID, color);
  }
}

int erase_figure(int v, int h, int figure, int rot){
  h = ini.h+h*pixel;
  v = ini.v+v*pixel;
  unsigned int color_erase = oled.RGB(0,0,0);
  
  for (int i = 0; i < 4; i++)
     oled.rectangle(h+scheme[figure][rot][i].h*pixel, v+scheme[figure][rot][i].v*pixel, pixel-1, pixel-1, OLED_SOLID, color_erase);
}

int show_intro(){
  oled.setfont(OLED_FONT5x7);
  oled.setfontmode(OLED_FONT_TRANSPARENT);
  
  /* Barrido de pantalla inicial */
  for (int i=0 ; i<128 ; i++) { oled.line(0, i, 159, i, oled.RGB(255-i*2, 0, 127+i)); }
  for (int i=0 ; i<128 ; i++) { oled.line(0, 127-i, 159, 127-i, oled.RGB(i*2, 255-i*2, 127+i)); }
  for (int i=0 ; i<160 ; i++) { oled.line(159-i, 0, 159-i, 127, oled.RGB(255-i, i, 0)); }
  for (int i=0 ; i<160 ; i++) { oled.line(i, 0, i, 127, oled.RGB(0, 255-i, 255-i)); }
  oled.Clear();
  
  oled.drawstringblock(5, 5, 0, oled.RGB(255, 255, 255), 2, 2, "Arduino 2560");
  oled.drawstringblock(5, 40, 0, oled.RGB(0, 255, 0), 1, 3, "Miquel Perello Nieto");
  oled.drawstringblock(5, 100, 0, oled.RGB(255, 0, 255), 1, 1, "www.perellonieto.com");
  delay(2000); 
  oled.Clear();
  return 1;
}

int show_menu_and_wait(){
  oled.rectangle(30, (3*45)+5, 100, 30, OLED_SOLID, oled.RGB(100, 255, 100));
  oled.drawstringblock(35, 25, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 2, 2, "TETRIS");
  oled.drawstringblock(60, 105, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 1, 1, "Start");
  
  while (digitalRead(PushButton) == HIGH){
    if (second%30 < 15) oled.drawstringblock(60, 105, OLED_FONT_OPAQUE, oled.RGB(255, 255, 255), 1, 1, "Start");
    else oled.drawstringblock(60, 105, OLED_FONT_OPAQUE, oled.RGB(0, 0, 0), 1, 1, "Start");
  }
  oled.Clear();
  return 1;
}

int show_game(){
  lines = 0;
  ini.h = 20;
  ini.v = 2;
  
  fig = random(N_FIG);
  // FIXME The previous random is always Zero
  fig = random(N_FIG);
  rot = random(N_ROT);
  pos.v = 0;
  pos.h = 3;
  time = 500;
  wait = 0;
  
  change_color();
  
   for (int i = 0; i < 20; i++)
    for (int j = 0; j < 10; j++)
      board[i][j] = -1;
   
  oled.SetBackground(oled.RGB(200, 100, 100));
  oled.rectangle(ini.h-1, ini.v-1, pixel*10+1, pixel*20+1, OLED_SOLID, oled.RGB(0,0,0));
  oled.rectangle(ini.h-1, ini.v-1, pixel*10+1, pixel*20+1, OLED_WIREFRAME, oled.RGB(255,255,255));
  paint_points();
  oled.drawstringblock(90, 25, OLED_FONT_OPAQUE, oled.RGB(255, 255, 150), 1, 2, "TETRIS");
  return 1; 
}

int limit(int v, int h, int fig, int rot){
  if (v > 20-fig_height(fig,rot))
    return 1;
  
  for (int i = 0; i < 4; i++)
    if (board[v+scheme[fig][rot][i].v][h+scheme[fig][rot][i].h] != -1) return 1;
  
  return 0;
}

void write_board_figure(int v, int h, int fig, int rot, unsigned int color){  
  for (int i = 0; i < 4; i++)
    board[v+scheme[fig][rot][i].v][h+scheme[fig][rot][i].h] = color;
}

void erase_board_line(int v){
  oled.rectangle(ini.h, ini.v+v*pixel, pixel*10-1, pixel-1, OLED_SOLID, oled.RGB(0,0,0));
  
  for (int i = v; i > 1; i--){
    for (int j = 0; j < 10; j++){
      board[i][j] = board[i-1][j];
      if (board[i][j] != -1){
        oled.rectangle(ini.h+j*pixel, ini.v+i*pixel, pixel-1, pixel-1, OLED_SOLID, board[i][j] & dark);
        oled.rectangle(ini.h+j*pixel+1, ini.v+i*pixel+1, pixel-3, pixel-3, OLED_SOLID, board[i][j]);
      }
      else
        oled.rectangle(ini.h+j*pixel, ini.v+i*pixel, pixel-1, pixel-1, OLED_SOLID, oled.RGB(0,0,0));
    }
  }
  
  
  for (int j = 0; j < 10; j++)
    board[0][j] = -1; 
  
  oled.rectangle(ini.h, ini.v, pixel*10-1, pixel-1, OLED_SOLID, oled.RGB(0,0,0));
}

void test_and_erase(){
  int borrar;
  
  for (int i = 19; i > 0; i--){
    borrar = 1;
    
    while (borrar == 1){
      for (int j = 0; borrar == 1 && j < 10; j++)
        if (board[i][j] == -1) borrar = 0;
      
      if (borrar == 1) {
        erase_board_line(i);
        lines++;
        paint_points();
      }
    }
  }
}

int can_put(int v, int h, int fig, int rot){
  for (int i = 0; i < 4; i++)
    if (board[v+scheme[fig][rot][i].v][h+scheme[fig][rot][i].h] != -1) return 0;
    
  return 1;
}

// Pick a new color for the next figure
void change_color(){
  color = figure_colors[random(N_FIG_COLORS)];
}

int show_game_over_and_wait(){
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
  
  for (int i = 0; i < 20; i++)
    for (int j = 0; j < 10; j++)
      board[i][j] = -1;
  
  while (digitalRead(PushButton) == HIGH);
  
  lines = 0;
  oled.rectangle(84, 50, 70, 62, OLED_SOLID, oled.RGB(200, 100, 100));
  oled.rectangle(ini.h-1, ini.v-1, pixel*10+1, pixel*20+1, OLED_SOLID, oled.RGB(0,0,0));
  oled.rectangle(ini.h-1, ini.v-1, pixel*10+1, pixel*20+1, OLED_WIREFRAME, oled.RGB(255,255,255));
  paint_points();
  oled.drawstringblock(90, 25, OLED_FONT_OPAQUE, oled.RGB(255, 255, 150), 1, 2, "TETRIS");
  return 1;
}

int setup_joystick(){
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
  
  return 1;
}

int setup_timer(){
  TIMSK1=0x01;    // enabled global and timer overflow interrupt;
  TCCR1A = 0x00;  // normal operation page 148 (mode0);
  TCNT1=0x0BDC;   // 16bit counter register
  TCCR1B = 0x04;  // start timer/ set clock
  return 1;
}

void setup(){
  Serial.begin(57600);  
  oled.Init();
  oled.SetContrast(CONTRAST);
  oled.Clear();
  
  randomSeed(analogRead(0));
  
  setup_joystick();
  setup_timer();
  
  //show_intro();
  //show_menu_and_wait();
  
  show_game();
}

void loop(){
  /* ================================ *
   * Calculando color para mostrar    *
   * ================================ */
  //if (pos.v > 20-fig_height(fig))
  if (limit(pos.v, pos.h, fig, rot) == 1){
    write_board_figure(ant.v, ant.h, fig, rot, color);
    
    test_and_erase();
    
    change_color();

    fig = random(N_FIG);
    rot = random(N_ROT);
    pos.v = 0;
    pos.h = 3;
    time = 500;
    wait = second;
    
    if (limit(pos.v, pos.h, fig, rot) == 1){
      show_game_over_and_wait();
      show_game();
    }
  }
  else{
    if (paint == 1) erase_figure(ant.v, ant.h, fig, ant_rot);
  }
  
    ant.h = pos.h;
    ant.v = pos.v;
    ant_rot = rot;
  
  if (paint == 1) {
    paint_figure(pos.v, pos.h, fig, rot, color);
    paint = 0;
  }
  
     /* Read joystick */
     if ((second - wait) > 10){
      if (digitalRead(UpButton) == LOW || digitalRead(PushButton) == LOW){ 
        aux = (rot+1)%4;
        if (pos.h < 11-fig_width(fig, aux) && can_put(pos.v, pos.h, fig, aux) == 1){
          rot = aux;
          paint = 1;
          wait = second;
        }
      }
      if (digitalRead(DownButton) == LOW) { 
        //pos.v++;
        //paint = 1;
        wait = second;
        second += 15;
      }
      if (digitalRead(LeftButton) == LOW) { 
        if (pos.h > 0 && can_put(pos.v, pos.h-1, fig, rot) == 1) {
          pos.h--;
          paint = 1;
          wait = second;
        }
      }
      if (digitalRead(RightButton) == LOW) { 
        if (pos.h < 10-fig_width(fig, rot) && can_put(pos.v, pos.h+1, fig, rot) == 1){
          pos.h++;
          paint = 1;
          wait = second;
        }
      }
    }
    
  if (second - ant_seconds > 50-lines/10){
    ant_seconds = second;
    pos.v++;
    paint = 1;
  }
}
