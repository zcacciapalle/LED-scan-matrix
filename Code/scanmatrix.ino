int refreshesPerFrame = 100; // proportional to inverse of framerate. Sorry about doing it weird, it makes more sense to do it this way.

// Here is your screen. If you want to display something, write it to here.
byte screen[8] = {  // test image for orientation. Spike in upper left corner, attached to top of screen.
  0b11111000,
  0b01111000,
  0b00111000,
  0b00011000,
  0b00001000,
  0b00000000,
  0b00000000,
  0b00000000
};


long long int font[] = {
0x000000000000,0x10E384100100,0x6DB480000000,0x00A7CA29F280,0x20E40C09C100,0x6590842134C0,0x214508552340,0x30C200000000, // !"#$%&'
0x108208208100,0x204104104200,0x00A39F38A000,0x00411F104000,0x00000000C308,0x00001F000000,0x00000000C300,0x001084210000, //()*+,-./
0x3914D5651380,0x10C104104380,0x3910462107C0,0x39104E051380,0x0862927C2080,0x7D041E051380,0x18841E451380,0x7C1084208200, //01234567
0x39144E451380,0x39144F042300,0x00030C00C300,0x00030C00C308,0x084210204080,0x0007C001F000,0x204081084200,0x391046100100, //89:;<=>?
0x3915D55D0380,0x3914517D1440,0x79145E451780,0x391410411380,0x791451451780,0x7D041E4107C0,0x7D041E410400,0x3914174513C0, //@ABCDEFG
0x45145F451440,0x384104104380,0x041041451380,0x452518512440,0x4104104107C0,0x45B551451440,0x459553451440,0x391451451380, //HIJKLMNO
0x79145E410400,0x391451552340,0x79145E491440,0x39140E051380,0x7C4104104100,0x451451451380,0x45145144A100,0x451555555280, //PQRSTUVW
0x451284291440,0x45144A104100,0x782108410780,0x388208208380,0x010204081000,0x382082082380,0x10A440000000,0x00000000003F, //XYZ[\]^_
0x30C100000000,0x0003813D13C0,0x410791451780,0x000391411380,0x0413D14513C0,0x000391790380,0x18821E208200,0x0003D144F04E, //`abcdefg
0x410712492480,0x100104104180,0x08018208248C,0x410494614480,0x104104104180,0x000695551440,0x000712492480,0x000391451380, //hijklmno
0x000791451790,0x0003D14513C1,0x000589208700,0x000390381380,0x00878820A100,0x000492496280,0x00045144A100,0x00045155F280, //pqrstuvw
0x000492312480,0x00049248E118,0x000782310780,0x188218208180,0x104100104100,0x302083082300,0x294000000000,0x10E6D145F000, //xyz{|}~⌂
0x05f41241041f,0x8ff000f9c03f,0x03cd3410413c                                                                             // '[ ° _ * ]'
};
//0x05f41241041f,0x8ff000f9c03f,0x03cd3410413c
char string[] = "   welcome to EHC! \x80\x81\x82   ";

int charCounter = 0;
int frameCounter = 8;
void frame(){ // "void loop" - can't call it "void loop" because im using loop to actually display whats on the screen
  charOut(string[charCounter+2], frameCounter+4);
  charOut(string[charCounter+1], frameCounter-2);
  charOut(string[charCounter], frameCounter-8);
  
  frameCounter--;
  if(frameCounter == 2){
    charCounter++;
    charCounter%=sizeof(string)/sizeof(char)-3;
    frameCounter = 8;
  }
}

void charOut(int index, int x){
  for(int y = 0; y < 8; y++){
    screen[y] &= ~((0b00111111<<10)>>(x+8));
    screen[y] |= ((((font[index-32]>>(42-6*y))&0b00111111)<<10)>>(x+8))&0b11111111;
  }
}



/**************************************
  * Leave everything below this alone
  * (unless you want to comment it)
**************************************/


int data = 2;
int clkH = 5;
int clkGND = 3;
int clkUpdate = 4;

int refreshCtr = 0;

void setup() {
  pinMode(data, OUTPUT);
  pinMode(clkH, OUTPUT);
  pinMode(clkUpdate, OUTPUT);
  pinMode(clkGND, OUTPUT);
}

void loop() {
  // this section makes sure the frame function gets called every now and then.
  refreshCtr++;
  if(refreshCtr >= refreshesPerFrame){
    refreshCtr = 0;
    frame();
  }

  // this section refreshes the screen a few times between frames.
  digitalWrite(data, HIGH); // first iteration pushes a ground signal into the shift register. Subsequent frames move that ground signal down the screen.
  for(int y = 0; y < 8; y++){

    digitalWrite(clkGND, LOW);   // move the ground signal down to the next row
    digitalWrite(clkGND, HIGH);

    for(int x = 0; x < 8; x++){   // shift in the data for the current row
      digitalWrite(data, (screen[y]>>x)&1);
      digitalWrite(clkH, LOW);
      digitalWrite(clkH, HIGH);
    }

    digitalWrite(clkUpdate, LOW);  // output the current row to the LEDs
    digitalWrite(clkUpdate, HIGH);

    digitalWrite(data, LOW); // prep the data line for the ground shift register. We don't want multiple 1s in the ground shift register at the same time.    
  }
}