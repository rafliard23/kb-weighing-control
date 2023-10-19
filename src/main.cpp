#include <Nextion.h>
#include <Arduino.h>

Nextion NextionBasic(Serial, 9600);

struct hmiPage{
  boolean isUserOn;
};
hmiPage selectMenu, menuAutomatic, menuManual, AutomaticMode;

struct process{
  boolean isDone;
};
process TaskPenakar, TaskPenuang, TaskKonveyor;

boolean bKonveyor;

/*
boolean bAuto;
boolean bManual;
boolean bAutoBack;
boolean bManualBack;
*/

String message;

// ---------------------- PIN DECLARATION ----------------------------------

const int Pi_penakar = 4;
const int Pi_penuang = 5;
const int Pi_konveyor = 6;
const int Pi_vibratorState = 11;
const int Pi_vibratorSpeed = 12;
const int Pi_ala1 = 8;  // Alarm 1 NO
const int Pi_ala2 = 9;  // Alarm 2 NC
const int Pi_prox = 10; // Proximity takar
const int Pi_proxStop = 3; // Proximity stop utk packing

// ---------------------- END OF VARIABLE DECLARATION ----------------------

/*
while (digitalRead(Pi_proxStop) == LOW && bKonveyor == HIGH)  {
  digitalWrite(Pi_konveyor, HIGH);
}
*/

//65 2 6 0 => stop
//65 2 5 0 => exit

//function exit
void function_Aexit() {
  NextionBasic.sendCommand("page page1");
  menuAutomatic.isUserOn = false;

  TaskPenakar.isDone = false;
  TaskPenuang.isDone = false;
  TaskKonveyor.isDone = false;
}

void function_Mexit() {
  NextionBasic.sendCommand("page page1");
  menuManual.isUserOn = false;

  TaskPenakar.isDone = false;
  TaskPenuang.isDone = false;
  TaskKonveyor.isDone = false;
}

void menu_Automatic() {
  while (menuAutomatic.isUserOn == true) {
    message = NextionBasic.listen();

    // Pencet Start
    if(message == "65 2 7 0 ffff ffff ffff") {
      AutomaticMode.isUserOn = true;
      while (AutomaticMode.isUserOn == true) {
        message = NextionBasic.listen();
        while(digitalRead(Pi_prox) == LOW && TaskKonveyor.isDone == false) {
          NextionBasic.setComponentText("t0", "Proses Menakar!");
          NextionBasic.sendCommand("t0.bco=YELLOW");
          NextionBasic.sendCommand("vis t0,1");

          digitalWrite(Pi_penakar, LOW);
          digitalWrite(Pi_vibratorState, LOW);
          digitalWrite(Pi_konveyor, HIGH);
          digitalWrite(Pi_penuang, HIGH);

          if(digitalRead(Pi_ala1) == HIGH) {
            digitalWrite(Pi_vibratorSpeed, HIGH);
          } else if(digitalRead(Pi_ala1) == LOW) {
            digitalWrite(Pi_vibratorSpeed, LOW);
          }

          if (digitalRead(Pi_ala1) == HIGH && digitalRead(Pi_ala2) == LOW) {
            TaskPenakar.isDone = true;
            NextionBasic.setComponentText("t0", "Penakar Selesai!");
            NextionBasic.sendCommand("t0.bco=GREEN");
          }

          while (TaskPenakar.isDone == true && TaskPenuang.isDone != true) {
            NextionBasic.setComponentText("t0", "Proses Menuang!");
            NextionBasic.sendCommand("t0.bco=YELLOW");
            digitalWrite(Pi_penakar, HIGH);
            digitalWrite(Pi_penuang, LOW);
            delay(1000);
            digitalWrite(Pi_konveyor, LOW);
            digitalWrite(Pi_penuang, HIGH);
            TaskPenuang.isDone = true;

            NextionBasic.setComponentText("t0", "Penuang selesai!");
            NextionBasic.sendCommand("t0.bco=GREEN");
            
            if(TaskPenuang.isDone == true) {
              delay(1000);
              NextionBasic.setComponentText("t0", "Konveyor on!");
              NextionBasic.sendCommand("t0.bco=YELLOW");

              digitalWrite(Pi_konveyor, LOW);
              delay(3500);

              TaskPenakar.isDone = false;
              TaskPenuang.isDone = false;
              TaskKonveyor.isDone = false;
            }

            if (message == "65 2 6 0 ffff ffff ffff") {
              AutomaticMode.isUserOn = false;
              TaskPenakar.isDone = false;
              TaskPenuang.isDone = false;
              TaskKonveyor.isDone = false;
            }
          }
        }        

        if (message == "65 2 6 0 ffff ffff ffff") {
          AutomaticMode.isUserOn = false;
          TaskPenakar.isDone = false;
          TaskPenuang.isDone = false;
          TaskKonveyor.isDone = false;
          }

        }
      }

    if (message == "65 2 5 0 ffff ffff ffff" && menuAutomatic.isUserOn == true) {
      // NextionBasic.buttonToggle(bAutoBack, "b0", 2, 5);
      NextionBasic.sendCommand("page page1");
      menuAutomatic.isUserOn = false;
    }


  } // end of while loop
} //end of function

// comment penuangan
/*
while (digitalRead(Pi_ala1) == HIGH && digitalRead(Pi_ala2) == HIGH && digitalRead(Pi_prox) == LOW) {
  digitalWrite(Pi_penakar, HIGH);
  digitalWrite(Pi_penuang, LOW);
  delay(2000);
  digitalWrite(Pi_penuang, HIGH);
  TaskPenuang.isDone = true;
}
*/
  
void menu_Manual() {
  while (menuManual.isUserOn == true) {
      message = NextionBasic.listen();
      
      //Fungsi kontrol penakar
      if (message == "65 3 2 0 ffff ffff ffff") {
        while (digitalRead(Pi_prox) == LOW && TaskPenakar.isDone != true) {
          // set title element properties
          NextionBasic.setComponentText("t0", "Proses Menakar!");
          NextionBasic.sendCommand("t0.bco=YELLOW");
          NextionBasic.sendCommand("vis t0,1");

          // set button element properties for being pressed and in process
          NextionBasic.sendCommand("b0.bco=GREEN");
          NextionBasic.sendCommand("b0.pco=WHITE");

          digitalWrite(Pi_penakar, LOW);
          digitalWrite(Pi_penuang, HIGH);
          digitalWrite(Pi_konveyor, HIGH);
          
          if (digitalRead(Pi_ala1) == HIGH) {
              digitalWrite(Pi_vibratorSpeed, HIGH);
          } else if (digitalRead(Pi_ala1) == LOW) {
            digitalWrite(Pi_vibratorSpeed, LOW);
          }
  
          if (digitalRead(Pi_ala1) == HIGH && digitalRead(Pi_ala2) == LOW) {
            digitalWrite(Pi_penakar, HIGH);
            TaskPenakar.isDone = true;
  
            NextionBasic.setComponentText("t0", "Penakar Selesai!");
            NextionBasic.sendCommand("t0.bco=GREEN");

            // set button element properties as unavailable and done
            NextionBasic.sendCommand("b0.bco=50712");
            NextionBasic.sendCommand("b0.pco=33808");
          }

          if (message == "65 3 5 0 ffff ffff ffff") {
            function_Mexit();
          }

        }
        // end of while loop
      }
      // end of penakar
  
      //Fungsi kontrol penuang
      if (message == "65 3 3 0 ffff ffff ffff") {
        if (TaskPenakar.isDone == true) {
          NextionBasic.setComponentText("t0", "Proses Menuang!");
          NextionBasic.sendCommand("t0.bco=YELLOW");

          // set button element properties for being pressed and in process
          NextionBasic.sendCommand("b1.bco=GREEN");
          NextionBasic.sendCommand("b1.pco=WHITE");

          digitalWrite(Pi_penuang, LOW);
          digitalWrite(Pi_penakar, HIGH);
          digitalWrite(Pi_konveyor, HIGH);
          delay(1500);
          
          NextionBasic.setComponentText("t0", "Penuang Selesai!");
          NextionBasic.sendCommand("t0.bco=GREEN");
          TaskPenuang.isDone = true;

          // set button element properties as unavailable and done
          NextionBasic.sendCommand("b1.bco=50712");
          NextionBasic.sendCommand("b1.pco=33808");
        }
      }
      // end of penuang
  
      //Fungsi kontrol konveyor
      if (message == "65 3 4 0 ffff ffff ffff") {
        NextionBasic.setComponentText("t0", "Konveyor On!");
        NextionBasic.sendCommand("t0.bco=YELLOW");

        // set button element properties for being pressed and in process
        NextionBasic.sendCommand("b2.bco=GREEN");
        NextionBasic.sendCommand("b2.pco=WHITE");

        digitalWrite(Pi_konveyor, LOW);
        digitalWrite(Pi_penuang, HIGH);
        digitalWrite(Pi_penakar, HIGH);
        while (TaskKonveyor.isDone == false) {
          if (digitalRead(Pi_prox) == LOW || digitalRead(Pi_proxStop) == LOW) {
            TaskKonveyor.isDone = true;
            NextionBasic.setComponentText("t0", "Konveyor Off!");
            NextionBasic.sendCommand("t0.bco=GREEN");
          }
        }

        // set button element properties as unavailable and done
        NextionBasic.sendCommand("b0.bco=50712");
        NextionBasic.sendCommand("b0.pco=BLACK");
        NextionBasic.sendCommand("b1.bco=50712");
        NextionBasic.sendCommand("b1.pco=BLACK");
        NextionBasic.sendCommand("b2.bco=50712");
        NextionBasic.sendCommand("b2.pco=BLACK");

        TaskPenakar.isDone = false;
        TaskPenuang.isDone = false;
        TaskKonveyor.isDone = false;
      }
      // end of konveyor
  
      //Fungsi tombol back
      if (message == "65 3 5 0 ffff ffff ffff" && menuManual.isUserOn == true) {
        // NextionBasic.buttonToggle(bManualBack, "b0", 3, 5);
        NextionBasic.sendCommand("page page1");
        menuManual.isUserOn = false;
  
        NextionBasic.sendCommand("vis t0,0");

      }

  // end of back function
  }
  // end of while loop section of manual function
}
// end of function manual


void main_menu() {
  message = NextionBasic.listen();
  TaskPenakar.isDone = false;
  digitalWrite(Pi_penakar, HIGH);
  digitalWrite(Pi_penuang, HIGH);
  digitalWrite(Pi_konveyor, HIGH);
  digitalWrite(Pi_vibratorState, LOW);
  
  // masuk menu automatic
  if (message == "65 1 2 0 ffff ffff ffff" && selectMenu.isUserOn == true) {
    // NextionBasic.buttonToggle(bAuto, "b2", 1, 2);
    NextionBasic.sendCommand("page page2");
    menuAutomatic.isUserOn = true;
  } else if (menuAutomatic.isUserOn == true) {
    menu_Automatic();

  }

  // masuk menu manual
  if (message == "65 1 3 0 ffff ffff ffff" && selectMenu.isUserOn == true) {
    // NextionBasic.buttonToggle(bManual, "b3", 1, 3);
    NextionBasic.sendCommand("page page3");
    menuManual.isUserOn = true;
  } else if (menuManual.isUserOn == true) {
    menu_Manual(); 

  }

}

// ---------------------- END OF SUBROUTINES -------------------------------

void setup() {
  pinMode(Pi_penakar, OUTPUT);
  pinMode(Pi_penuang, OUTPUT);
  pinMode(Pi_konveyor, OUTPUT);
  pinMode(Pi_vibratorState, OUTPUT);
  pinMode(Pi_vibratorSpeed, OUTPUT);
  pinMode(Pi_ala1, INPUT);
  pinMode(Pi_ala2, INPUT);
  pinMode(Pi_prox, INPUT);
  pinMode(Pi_proxStop, INPUT);

  digitalWrite(Pi_vibratorSpeed, LOW);
  digitalWrite(Pi_vibratorState, LOW);

  // memastikan untuk mematikan penakar, penang dan konveyor tidak nyala ketika start
  // penakar, penuang, konveyor active low
  digitalWrite(Pi_penakar, HIGH);
  digitalWrite(Pi_penuang, HIGH);
  digitalWrite(Pi_konveyor, HIGH);
  digitalWrite(Pi_vibratorState, LOW);

  // set flag dengan inisialisasi tiap proses belum menyala
  TaskPenakar.isDone = false;
  TaskPenuang.isDone = false;
  TaskKonveyor.isDone = false;
  
  Serial.begin(9600);
  NextionBasic.init();

  delay(2000);
  NextionBasic.sendCommand("page page1");
  selectMenu.isUserOn = true;
}

void loop() {
  main_menu();
}

// ---------------------- END OF CODE ------------------------------------
