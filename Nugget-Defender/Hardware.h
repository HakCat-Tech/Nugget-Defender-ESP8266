#define upBtn D6 // left button
#define dnBtn D3 // right button
#define ltBtn D7 // left button
#define rtBtn D5 // right button

// button states and previous states
int uState = 0; int puState = 1; 
int dState = 0; int pdState = 1;
int lState = 0; int plState = 1; 
int rState = 0; int prState = 1;


// return which button pressed
uint8_t buttonsPressed() {
  uState = digitalRead(upBtn);
  dState = digitalRead(dnBtn);
  lState = digitalRead(ltBtn);
  rState = digitalRead(rtBtn);

  if      (uState == 0) {return 1;} // up
  else if (dState == 0) { return 2;} // down
  else if (lState == 0) return 3; // left
  else if (rState == 0) return 4; // right

  return 0;  
}
