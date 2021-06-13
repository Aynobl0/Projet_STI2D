//Inclusion des fonctions
#include "headers.h"

//Inclusion des librairies
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//Création de la commande 'loopfor()'
/*
 * loopfor(nb) repète nb fois l'action que la commande englobe
 */
#define loopfor(nb) for (int _loop = 0, _max = (nb); _loop <= _max; _loop++)

int etatMenu=-1; //etatMenu=-1 -> Ecran de Bvn
                 //etatMenu=0 -> Menu principal activé
                 //etatMenu=1 -> Menu GPS activé
                 //etatMenu=4 -> GPS activé
                 //etatMenu=2 -> Menu test activé
                 //etatMenu=3 -> Menu test GPS activé
int lastEtatMenu;

//char c='1';
//int n='1';
int ID

int etatChoix=1;
int lastEtatChoix=1;

int etatNav=0;
int lastEtatNav=0;

int etatTestMoteur=0; //etatTestMoteur>0 -> moteur tourne.
                      //etatTestMoteur=1 -> moteur tourne dans un sens.
                      //etatTestMoteur=2 -> moteur tourne dans l'autre sens.

int etatServo=0;
int lastEtatServo=0;

void setup() {
  Serial.begin(9600); //Lancement de la console de debugage
  lcd.begin(16, 2); //Déclaration des dimensions de l'écran
  //Affichage sur 2 lignes du Bienvenue.
  lcd.setCursor(0,0);
  lcd.print("Bienvenue dans");
  lcd.setCursor(0,1);
  lcd.print("le CleanerBoat");
  delay(1500);
  lcd.clear();
  
  lastEtatMenu=etatMenu;
  etatMenu=0;
}

//Initialisation du clavier.
uint8_t i=0;

void loop() {
  //Actualisations
  if(lastEtatMenu!=etatMenu) {
    lcd.clear();
    lastEtatMenu=etatMenu;
  }
  if(lastEtatChoix!=etatChoix) {
    lcd.clear();
    lastEtatChoix=etatChoix;
  }
  if(lastEtatNav!=etatNav) {
    lcd.clear();
    lastEtatNav=etatNav;
  }
  if(lastEtatServo!=etatServo) {
    if(etatNav==1) {
      lcd.setCursor(8,1);
      lcd.print("   ");
    } else if(etatNav==2) {
      lcd.setCursor(8,0);
      lcd.print("   ");
    }
    lastEtatServo=etatServo;
  }

  //Affichage des differents menu
  if(etatMenu==0) {
    lcd.setCursor(0,0);
    lcd.print("Entrer coords >");
    lcd.setCursor(0,1);
    lcd.print("Verifications >");
    if(etatChoix==1) {
      lcd.setCursor(15,0);
      lcd.print("-");
    } else {
      lcd.setCursor(15,1);
      lcd.print("-");
    }
  }
  if(etatMenu==1) {
    lcd.setCursor(0,0);
    lcd.print("Entrer les coord");
    lcd.setCursor(0,1);
    lcd.print("de la zone");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Latitude en haut");
    lcd.setCursor(0,1);
    lcd.print("Longitude en bas");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Appuyer sur bas");
    lcd.setCursor(0,1);
    lcd.print("pour commencer");
    delay(2000);
    lcd.clear();
  }
  if(etatMenu==2) {
    lcd.setCursor(0,0);
    if(etatNav==0) {
      lcd.print("Tests:");
      lcd.setCursor(0,1);
      lcd.print("Moteur - ");
      lcd.setCursor(9,1);
      if(etatTestMoteur==0) {
        lcd.print("OFF ");
      } else if(etatTestMoteur==1) {
        lcd.print("ON >");
      } else {
        lcd.print("ON <");
      }
    }else if(etatNav==1) {
      lcd.print("Moteur - ");
      lcd.setCursor(9,0);
      if(etatTestMoteur==0) {
        lcd.print("OFF ");
      } else if(etatTestMoteur==1) {
        lcd.print("ON >");
      } else {
        lcd.print("ON <");
      }
      lcd.setCursor(0,1);
      lcd.print("Servo - ");
      lcd.setCursor(8,1);
      lcd.print(etatServo);
   } else if(etatNav==2) {
      lcd.print("Servo - ");
      lcd.setCursor(8,0);
      lcd.print(etatServo);
      lcd.setCursor(0,1);
      lcd.print("UltraSon - ");
      lcd.setCursor(11,1);
      lcd.print(testUS());
   } else if(etatNav==3){
      lcd.print("UltraSon - ");
      lcd.setCursor(11,0);
      lcd.print(testUS());
      lcd.setCursor(0,1);
      lcd.print("Capteur - ");
      lcd.setCursor(10,1);
      lcd.print(testCapteur());
   } else if (etatNav==4) {
      lcd.print("Capteur - ");
      lcd.setCursor(10,0);
      lcd.print(testCapteur());
      lcd.setCursor(0,1);
      lcd.print("GPS");
   }
      if(etatChoix==1) {
      lcd.setCursor(15,0);
      lcd.print("-");
    } else if(etatChoix==2) {
      lcd.setCursor(15,1);
      lcd.print("-");
    }
  }
  if(etatMenu==3) {
    lcd.setCursor(0,0);
    lcd.print(testGPSLatitude());
    lcd.setCursor(0,1);
    lcd.print(testGPSLongitude());
  }

  //Lancement de la lecture des boutons.
  uint8_t buttons = lcd.readButtons();

  //Association des boutons, selon l'endroit du menu à une action.
  lcd.setCursor(0,0);
  if (buttons & BUTTON_UP) { //Appui sur le bouton "Haut"
    Serial.println("Boutton Haut appuyé");
    if(etatMenu==0) {
      if(etatChoix==2) {
        etatChoix=1;
      }
    }
    if(etatMenu==1) {
      
    }
    if(etatMenu==2) {
      if(etatChoix==2){
        etatChoix=1;
      } else if(etatChoix==1) {
        if(etatNav>0) {
         etatNav--;
        }
      }
    }
    delay(50);
  }
  if (buttons & BUTTON_DOWN) { //Appui sur le bouton "Bas"
    Serial.println("Boutton Bas appuyé");
    //Bas
    if(etatMenu==0) {
      if(etatChoix==1) {
        etatChoix=2;
      }
    }
    if(etatMenu==1) {
      
    }
    if(etatMenu==2) {
      if(etatChoix==1){
        etatChoix=2;
      } else if(etatChoix==2) {
        if(etatNav<4) {
         etatNav++;
        }
      }
    }
    delay(50);
  }
  if (buttons & BUTTON_LEFT) { //Appui sur le bouton "Gauche"
    Serial.println("Boutton Gauche appuyé");
    if((etatMenu==1 && ) || etatMenu==2) {
      etatMenu=0;
    } else if(etatMenu==3) {
      etatMenu=2;
    }
    delay(50);
  }
  if (buttons & BUTTON_RIGHT) { //Appui sur le bouton "Droit"
    Serial.println("Boutton Droite appuyé");
    if(etatMenu==0) {
      if(etatChoix==1) {
        etatMenu=1;
      } else {
        etatMenu=2;
        etatChoix=1;
        etatNav=0;
      }
    }
    if(etatMenu==1) {
      
    }
    if(etatMenu==2) {
      if((etatNav==0 && etatChoix==2) || (etatNav==1 & etatChoix==1)) {
        if(etatTestMoteur==0) {
          etatTestMoteur=1;
        } else if(etatTestMoteur==1) {
          etatTestMoteur=2;
        } else {
          etatTestMoteur=0;
        }
      } else if((etatNav==1 & etatChoix==2) || (etatNav==2 & etatChoix==1)) {
        if(etatServo==0) {
          etatServo=30;
        } else if(etatServo==30) {
          etatServo=60;
        } else if(etatServo==60) {
          etatServo=90;
        } else if(etatServo==90) {
          etatServo=120;
        } else if(etatServo==120) {
          etatServo=150;
        } else if(etatServo==150) {
          etatServo=180;
        } else if(etatServo==180) {
          etatServo=0;
        } 
      }else if(etatNav==4 & etatChoix==2) {
        etatMenu=3;
      }
    }
    delay(50);
  }
  if (buttons & BUTTON_SELECT) { //Appui sur le bouton "Select"
    etatMenu=0;
  }
}
