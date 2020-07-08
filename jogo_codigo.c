#include <LiquidCrystal.h>
#include <string.h>

#define butArrow 9 
#define butSelection 10
#define tilt 13

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

int line[5]={1,2,3,4,5},
    linreturn[5],
    indice, 
    number=1, 
    score=0,
	i, 
	j, 
	k, 
	a=0, 
	currentplayer=3;

bool menu=0, 
	 enter=0, 
	 submenu=0, 
	 but=0, 
	 colision;

//CustomChar
byte ghost[8] = {
  B01110,
  B11111,
  B10101,
  B11111,
  B11111,
  B11111,
  B11111,
  B10101};

byte ghost2[8] = {
  B01110,
  B11111,
  B10101,
  B11111,
  B11111,
  B11111,
  B11111,
  B01010};

byte cloudA[8] = {
  B00000,
  B00000,
  B00000,
  B01101,
  B11111,
  B11111,
  B01111,
  B00000
};

byte cloudB[8] = {
  B00000,
  B00000,
  B11000,
  B11100,
  B11110,
  B11110,
  B11100,
  B00000
};

byte cloudC[8] = {
  B00000,
  B00000,
  B01110,
  B11111,
  B11111,
  B11111,
  B01111,
  B00000
};

byte cloudD[8] = {
  B00000,
  B00000,
  B01100,
  B11110,
  B11111,
  B11111,
  B11110,
  B00000
};

byte grave[8] = {
  B00000,
  B00000,
  B01110,
  B11011,
  B10001,
  B11011,
  B11011,
  B11111
};

byte tree[8] = {
  B10011,
  B10110,
  B11100,
  B00101,
  B00111,
  B11100,
  B00100,
  B00100
};

struct obstacles{
  int px;
  int py;
  int type;
  } clouds[6];

struct signUp{
  int score=0;
  } player[3];

void printanimation(); //164
void printmenu(); //222
void readbuttons(); //251
void listmenu(); //268
void selectionmenu(); //281
void signUp(); //301
void ranking(); //353
void credits(); //380
void instructions(); //397
void start(); //479
void randomcloud(); //536
void printcloud(int pxcloud, int pycloud, int cloudtype); //544
void star(); //557
void gameover(); //620
void youwin(); //631

void setup(){
  Serial.begin(9600);
  lcd.createChar(0, ghost);
  lcd.createChar(1, ghost2);
  lcd.createChar(2, cloudA);
  lcd.createChar(3, cloudC);
  lcd.createChar(4, cloudB);
  lcd.createChar(5, cloudD);
  lcd.createChar(6, grave);
  lcd.createChar(7, tree);
  lcd.begin(16, 4);
  pinMode(butArrow, INPUT);
  pinMode(butSelection, INPUT);
  pinMode(tilt, INPUT_PULLUP);
  printanimation();
  lcd.clear();
}
  
void loop (){
  if (!submenu){
    printmenu();
  }
}

void printanimation(){
  lcd.setCursor(1 , 0);
  lcd.write(byte(2));
  lcd.write(byte(4));
  lcd.setCursor(5 , 0);
  lcd.write(byte(3));
  lcd.write(byte(5));
  lcd.setCursor(11 , 0);
  lcd.write(byte(2));
  lcd.write(byte(4));
  lcd.setCursor(3 , 1);
  lcd.write(byte(6));
  lcd.setCursor(7 , 1);
  lcd.write(byte(7));
  lcd.setCursor(10 , 1);
  lcd.write(byte(6));
  lcd.setCursor(13 , 1);
  lcd.write(byte(7));
  //move ghost
  for(int i=15; i>-15;i--){
    if (i%2==0){
      lcd.setCursor(i+8 , 0);
      lcd.write(byte(0));
    } else {
      lcd.setCursor(i+8 , 0);
      lcd.write(byte(1));
    }
    lcd.setCursor(i+9 , 0);
    lcd.print(" ");
    //print cloud after ghost pass
    if (i+9==11){
      lcd.setCursor(11 , 0);
      lcd.write(byte(2));
    }
    if (i+9==12){
      lcd.setCursor(12 , 0);
  	  lcd.write(byte(4));
    }
    if (i+9==5){
      lcd.setCursor(5 , 0);
      lcd.write(byte(3));
    }
    if (i+9==6){
      lcd.setCursor(6 , 0);
  	  lcd.write(byte(5));
    }
    if (i+9==1){
      lcd.setCursor(1 , 0);
      lcd.write(byte(2));
    }
    if (i+9==2){
      lcd.setCursor(2 , 0);
      lcd.write(byte(4));
    }
    delay(300);
  }
}

void printmenu(){
    for(int i=15; i>-15;i--){
        lcd.setCursor(i , 0);
        lcd.print("FLAPPY GHOST ");
		if (i%2==0){
          lcd.setCursor(i+13 , 0);
          lcd.write(byte(0));
         } else {
          lcd.setCursor(i+13 , 0);
          lcd.write(byte(1));
         }
        lcd.setCursor(i+14 , 0);
        lcd.print("   ");
        lcd.setCursor(0,1);
        lcd.print(">>");
        lcd.setCursor(2,line[5]);
        lcd.print("Start         ");
        lcd.setCursor(2,line[1]);
        lcd.print("Player        ");
        lcd.setCursor(2,line[2]);
        lcd.print("Ranking       ");
        lcd.setCursor(2,line[3]);
        lcd.print("Instructions  ");
        lcd.setCursor(2,line[4]);
        lcd.print("Credits       ");
		readbuttons();
	}
}

void readbuttons(){
  if (!digitalRead(butArrow)) menu = 1;
  if (!digitalRead(butSelection)) enter = 1;
  if (digitalRead(butArrow) && menu){
  	menu = 0;
    listmenu();
    number = number + 1;
    if (number>5) number = 1;
  }
  if (digitalRead(butSelection) && enter) {
  	enter = 0;
    submenu = !submenu;
    selectionmenu();
  }
}

//Rotation of menu's opitions
void listmenu(){
	int i;
 	for (i=4;i>-1;i--){
  		indice = i - 1;
      	linreturn[i] = line[i];
      	if (indice<0){
      		line[i] = linreturn[i+4];
        } else {
        	line[i] = line[i-1];
        }
 	}
}

void selectionmenu(){
  switch (number){
  	case 1:
    	start();
    	break;
    case 2:
    	signUp();
    	break;
    case 3:
    	raking();
    	break;
    case 4:
    	instructions();
    	break;
    case 5:
    	credits();
    	break;
  }
}

//função para escolha do player
void signUp(){
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Player Selection");
	int p_pos = 0;
	bool exit = true;
  
	while(exit){  
      if (!digitalRead(butArrow));
  	  delay(150);
		if(p_pos==0){
          lcd.setCursor(0,1);
          lcd.print("Casper      ");
          if(digitalRead(butSelection)){
            currentplayer=0;
            exit = false;
          }
          if(digitalRead(butArrow)) {
              p_pos += 1;
              delay(20);
          }
		}else if (p_pos==1){
          lcd.setCursor(0,1);
          lcd.print("Murta Warren");
		  if(digitalRead(butSelection)){
            currentplayer=1;
            exit = false;
          }
           if(digitalRead(butArrow)) {
              p_pos += 1;
              delay(20);
           }
		}else if (p_pos==2){
          lcd.setCursor(0,1);
          lcd.print("Slimer      ");
    	  if(digitalRead(butSelection)) {
            currentplayer=2;
          	exit = false;
          }
          if(digitalRead(butArrow)) {
             p_pos += 1;
             delay(20);
          }
		}else if (p_pos >2){
           p_pos=0;
        }
	}
	if (submenu) submenu = !submenu;
    lcd.clear();
}

void raking(){
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("RANKING");
    delay(300);
    lcd.setCursor(1,1);
    lcd.print("Casper");
    lcd.print(" - ");
    lcd.print(player[0].score);
    lcd.print("     ");
    delay(2000);
    lcd.setCursor(1,1);
    lcd.print("Murta");
    lcd.print(" - ");
    lcd.print(player[1].score);
    lcd.print("      ");
    delay(2000);
    lcd.setCursor(1,1);
    lcd.write("Slimer");
    lcd.print(" - ");
    lcd.print(player[2].score);
    lcd.print("     ");
    delay(2000);
    lcd.clear();
    if (submenu) submenu = !submenu;
}

void credits(){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("DEVELOPER");
    lcd.setCursor(0,1);
    lcd.print("Thaina W. Chagas");
    delay(2000);
    lcd.clear();
    if (submenu) submenu = !submenu;
}

void instructions(){
    lcd.clear();
    delay(300);
    lcd.setCursor(0,0);
    lcd.print("  INSTRUCTIONS  ");
    lcd.setCursor(0,1);
    lcd.print("Incline the tilt");
    delay(2500);
    lcd.setCursor(0,0);
    lcd.print("to make the     ");
    lcd.setCursor(0,1);
    lcd.print("Ghost fly up or ");
    delay(2500);
    lcd.setCursor(0,0);
    lcd.print("down, dodging   ");
    lcd.setCursor(0,1);
    lcd.print("the clouds.     ");
    delay(2500);
    lcd.setCursor(0,0);
    lcd.print("For each cloud  ");
    lcd.setCursor(0,1);
    lcd.print("deflected,      ");
    delay(2500);
    lcd.setCursor(0,0);
    lcd.print("+1 is scored.   ");
    lcd.setCursor(0,1);
    lcd.print("If bump into a  ");
    delay(2500);
    lcd.setCursor(0,0);
    lcd.print("cloud Ghost will");
    lcd.setCursor(0,1);
    lcd.print("be raised and   ");
    delay(2500);
    lcd.setCursor(0,0);
    lcd.print("its a game over.");
    lcd.setCursor(0,1);
    lcd.print("Play with your  ");
    delay(2500);
    lcd.setCursor(0,0);
    lcd.print("friends choosing");
    lcd.setCursor(0,1);
    lcd.print("between players ");
    delay(2500);
    lcd.setCursor(0,0);
    lcd.print("and see who gets");
    lcd.setCursor(0,1);
    lcd.print("the highest score");
    delay(2500);
    lcd.clear();
    if (submenu) submenu = !submenu;
}

void start(){
    lcd.clear();
    colision=1;
    score=90;
    int delaycloud = 50;
	randomcloud();
  
    //verify if the player is select
    if (currentplayer==3) signUp();
    
    //read buttons
    if (!digitalRead(tilt)) but = 1;
    if (digitalRead(tilt) && but) {
      but = 0;
    }
  
	while (colision){
		lcd.setCursor(14,0);
		lcd.print(score); 
      
        //Chama o obstaculo e faz ele se mover até o inicio da tela
      	for (a=0;a<6;a++){
          if (clouds[a].px < 50) {
            printcloud(clouds[a].px,clouds[a].py, clouds[a].type);
            clouds[a].px--;
          }
          if (clouds[a].px==-2){
            score++;
          }
          if (clouds[a].px<-2 && score < 94){
             clouds[a].px = 22;
             clouds[a].py = random(0,2);
          } else if (clouds[a].px<-2 && score >= 94){
          	clouds[a].px = 55;
          } 
        }
        if(delaycloud > 5){
          delaycloud--;
        }
        delay(delaycloud);
      
         //Ghost position
         while(but==1){
            lcd.setCursor(1,0);
            lcd.write(byte(0));
            delay(200);
            lcd.setCursor(1,0);
            lcd.write(byte(1));
            delay(200);
            if (digitalRead(tilt) && but) {
                but = 0;
                lcd.setCursor(1,0);
                lcd.print(" ");
            } 
            break;
          }
          while(but==0){
            lcd.setCursor(1,1);
            lcd.write(byte(0));
            delay(200);
            lcd.setCursor(1,1);
            lcd.write(byte(1));
            delay(200);
            if (!digitalRead(tilt)) {
                but = 1;
                lcd.setCursor(1,1);
                lcd.print(" ");
            }
            break;
          }

          //Verifica if colide with a cloud
          for (a=0;a<6;a++){
            if ((but == 1 && clouds[a].px ==1 && clouds[a].py == 0) || (but == 1 && clouds[a].px ==0 && clouds[a].py == 0) ||(but == 0 && clouds[a].px == 1 && clouds[a].py == 1)||(but == 0 && clouds[a].px == 0 && clouds[a].py == 1)) {
                gameover();
                colision=0;
                clouds[a].px = 16;
            }
          }
          //Verify if catch the star
          if (score == 99){
            star();
            colision=0;
          }
    }
    if(player[currentplayer].score < score){
    	player[currentplayer].score = score;
    }
	if (submenu) submenu = !submenu;
}

void randomcloud(){
  for (i=0;i<6;i++){
  	clouds[i].py = random(0,2);
	clouds[i].px = (15 + (i*4));
	clouds[i].type = random(2,4);
  }
}

void printcloud(int pxcloud, int pycloud, int cloudtype){ 
    lcd.setCursor(pxcloud,pycloud);
    lcd.write(byte(cloudtype));
    lcd.setCursor(pxcloud+1,pycloud);
    if (cloudtype==2) {
      lcd.write(byte(4));
    } else if (cloudtype==3) {
      lcd.write(byte(5));
    }
    lcd.setCursor(pxcloud+2,pycloud);
    lcd.write(" ");
}

void star(){
	lcd.clear();
    colision=1;
    int starX=16;
    int starY=random(0,2);
	
    //read buttons
    if (!digitalRead(tilt)) but = 1;
    if (digitalRead(tilt) && but) {
      but = 0;
    }
   
	while (colision){
		lcd.setCursor(14,0);
		lcd.print(score); 
      	
      	// printStar
      	lcd.setCursor(starX, starY);
      	lcd.print("* ");
           
       //Ghost position
       while(but==1){
          lcd.setCursor(1,0);
          lcd.write(byte(0));
          delay(200);
          lcd.setCursor(1,0);
          lcd.write(byte(1));
          delay(200);
          if (digitalRead(tilt) && but) {
              but = 0;
              lcd.setCursor(1,0);
              lcd.print(" ");
          } 
          break;
		}
		while(but==0){
          lcd.setCursor(1,1);
          lcd.write(byte(0));
          delay(200);
          lcd.setCursor(1,1);
          lcd.write(byte(1));
          delay(200);
          if (!digitalRead(tilt)) {
              but = 1;
              lcd.setCursor(1,1);
              lcd.print(" ");
          }
          break;
		}
      
        //Verify if catch the star
        if ((but == 1 && starX ==1 && starY == 0) || (but == 0 && starX == 1 && starY == 1)) {
          	youwin();
        	colision = 0;	
        } else if (starX == -1){
          gameover();
          colision = 0;
        }
       starX--;
    }
	if (submenu) submenu = !submenu;
}

void gameover(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("YOU HAVE RISEN:(");
  lcd.setCursor(0,1);
  lcd.print("SCORE: ");
  lcd.print(score);
  delay(3000);
  lcd.clear(); 
}

void youwin(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("FOUND THE LIGHT!");
  lcd.setCursor(0,1);
  lcd.print("SCORE: ");
  lcd.print(score);
  delay(3000);
  lcd.clear();
}