//Ons Bakske - Made by Andries Demeulenaere & Ferre Lambert

  /////////////////
 /// LIBRARIES ///
/////////////////
#include <LiquidCrystal.h>
//CODE FOR INTERFACE & MEMORY
#include "MegunoLink.h"
#include "CommandHandler.h"
#include <EEPROMex.h>

  ////////////////////////
 /// GLOBAL VARIABLES ///
////////////////////////

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //Set the right configured LCD pins
bool Phone_Left = false;                //True if there is a phone on the left LDR
bool Phone_Right = false;               //True if there is a phone on the right LDR
String txt1;                            //Two strings to split 16x1 characters into 8x2
String txt2;                            //Two strings to split 16x1 characters into 8x2
int oldState = 0;                       //Two states to compare if the program needs to do anything
int newState = 1;                       //Two states to compare if the program needs to do anything
int LED_Left = 2;                       //Digital Pin 2
int LED_Right = 3;                      //Digital Pin 3
int LDR_Left = 1;                       //Analog Pin 1
int LDR_Right = 2;                      //Analog Pin 2
int LDR_Value_Left = 0;                 //Incoming Value of left LDR
int LDR_Value_Right = 0;                //Incoming Value of left LDR
int Light_sensitivity = 130;            //Sensitivity of the LDR's (when is dark enough to do something)
int i;                                  //Variable used for the loops
int address = 1;                        //Address needed for EEPROM memory
int subjectNumber = 1;                  //Variable used as subject number of array
bool appIsStarting;                     //Variable used to check if app is in start-up
bool reset;                             //Variable used to check if app is resetted

//hard-coded string array to test
char* subjects[]={
  "Charlie Hebdo",
  "Adobe CC",
  "KdG Labs",
  "Conchita Wurst",
  "The Hobbit",
  "Geluk",
  "11-09-2001",
  "Twitter",
  "Facebook",
  "Kwantumfysica",
  "Van Gogh",
  "Kerstmis",
  "Australie",
  "New York",
  "Burger King",
  "MCDonalds",
  "The Beatles",
  "Koken",
  "Huisdieren",
  "Voetbal",
  "MCT",
  "Arduino",
  "Javascript",
  "HTML5",
  "Web Design",
  "Pokémon",
  "Coca-Cola",
  "Obamacare",
  "The Hunger Games",
  "Doctor Who",
  "Windows",
  "Apple"};

//CODE FOR INTERFACE & MEMORY

//string to output to (used in READ-function)
char output[] = "       ";

//needed by MEGUNOLINK
CommandHandler<> SerialCommandHandler;

//Get-function used by MegUNOLink
/*void Cmd_GetAllSubjects(CommandParameter &Parameters)
{ 
  //print subject to MEGUNOLINK via Parameters.GetSource().println(..);
  
   for(int k = 0; k<=512;k++){
      int einde = k + 16;
      //EEPROM.readBlock<char>(k, output, 16);
      //Parameters.GetSource().print("Plaats ");
      
      int plaats = k/16+1; //user friendly numbers
      
      //Parameters.GetSource().print(plaats);
      //Parameters.GetSource().print(" : ");
      
      //Parameters.GetSource().println(output);
      if(EEPROM.read(k) != 65){
        EEPROM.write(k, 65);
        Serial.println("VERANDER");
        }
      }    
}

void Cmd_SetSubject(CommandParameter &Parameters)
{
  Serial.println(Parameters.NextParameter());
  Serial.println(address);
  
  EEPROM.writeBlock<char>(1, Parameters.NextParameter(), 16);
  EEPROM.readBlock<char>(1, output, 16);
  Serial.println(output);
  
  
}

void Cmd_GetRndmSubject(CommandParameter &Parameters){
  
  //randomize the i for a random subject
    randomize();
    
    //EEPROM.readBlock<char>(i, output, 16);
    Serial.println(EEPROM.read(0));
    Parameters.GetSource().print("Random onderwerp: ");
    Parameters.GetSource().println(output);
    
  }
  
void Cmd_SetAddress(CommandParameter &Parameters){
    address = (int)atoi(Parameters.NextParameter());
    address = (address - 1)*16;
    //Serial.println(address);
}
*/



//Setup (or startup) function
void setup()
{
  Serial.begin(9600);
  
  //App is in start-up mode
  appIsStarting = true;
  
  //Initialise LED outputs
  pinMode(LED_Left, OUTPUT);
  pinMode(LED_Right, OUTPUT);
  
  //Initialise LCD
  lcd.begin(8, 2);     //is 16x1, adressed as 8x2
  lcd.setCursor(0,1);  //init right hand side
  lcd.home();          //back to start
  lcd.clear();         //clear screen
  
  //CODE FOR INTERFACE & MEMORY
 /* SerialCommandHandler.AddCommand(F("SetSubject"), Cmd_SetSubject);
  SerialCommandHandler.AddCommand(F("GetAllSubjects"), Cmd_GetAllSubjects);
  SerialCommandHandler.AddCommand(F("GetRndmSubject"), Cmd_GetRndmSubject);
  SerialCommandHandler.AddCommand(F("SetAddress"), Cmd_SetAddress);
 */
}

void randomize(){
  int m = random(0,2);
  i = m*16;
}

//Loop function
void loop()
{
  //CODE FOR INTERFACE & MEMORY
  //SerialCommandHandler.Process();
  
  if(appIsStarting)
  {
    //Start loading
    int counter = 0;
    txt1 = "Starting";
    while(counter < 4){
      txt2 = "";
      printToLCD(txt1, txt2);
      delay(250);
      txt2 = ".";
      printToLCD(txt1, txt2);
      delay(250);
      txt2 = "..";
      printToLCD(txt1, txt2);
      delay(250);
      txt2 = "...";
      printToLCD(txt1, txt2);
      delay(250);
      counter++;
    }
    appIsStarting = false;
    txt1 = "Welkom!";
    txt2 = "";
    printToLCD(txt1, txt2);
    delay(4000);
  }
  else
  {
    //Read LDR Values 
    LDR_Value_Left = analogRead(LDR_Left);
    LDR_Value_Right = analogRead(LDR_Right);
    
    //Serial print them for testing purposes
    //Serial.println(LDR_Value_Left);
    //Serial.println(LDR_Value_Right);
    
    //Check if left phone is placed
    if(LDR_Value_Left < Light_sensitivity)
    {
      Phone_Left = true;
    }
    else
    {
      Phone_Left = false;
    }
    
    //Check if right phone is placed
    if(LDR_Value_Right < Light_sensitivity)
    {
      Phone_Right = true;
    }
    else
    {
      Phone_Right = false;
    }
    
    //Check if there are no phones placed
    if(!Phone_Left && !Phone_Right)
    {      
      newState = 1;
    }
    //Check if only left phone is placed
    else if(Phone_Left && !Phone_Right)
    {
      newState = 2;
    }
    //Check if only right phone is placed
    else if(!Phone_Left && Phone_Right)
    {      
      newState = 3;
    }
    //Check if both phones are placed
    else if(Phone_Left && Phone_Right)
    {
      newState = 4;
      //If placed here, because it cant be placed in switch
      //Only if reset is true, then print a new subject (different from the last one)
      if(reset)
      {
        int oldSubjectNumber = subjectNumber;
        while(subjectNumber == oldSubjectNumber)
        {
          //get length of subjects array
          int arrSize = 32; //sizeof(subjects) / sizeof(int);

          subjectNumber = random(0,arrSize);
          //Serial.println("random is used");  
          //Serial.println(subjectNumber);
        }
        reset = false;
      }
    }
    else
    {
      //Error occured
      newState = 0;
    }
    
    if(oldState != newState)
    {
      if (oldState == 4)
      {
        txt1 = "Dank! #O";
        txt2 = "NSBAKSKE";
        printToLCD(txt1, txt2);
        delay(7000);
      }
      //Change state and execute lcd code based on stage based on state number
      oldState = newState;
      switch (newState) {
        case 1:
          //No phones placed
          digitalWrite(LED_Left, LOW);
          digitalWrite(LED_Right, LOW);
          txt1 = "Plaats u";
          txt2 = "w GSM's";
          printToLCD(txt1, txt2);
          reset = true;
          break;
        case 2:
          //Only left phone placed
          digitalWrite(LED_Left, HIGH);
          digitalWrite(LED_Right, LOW);
          txt1 = "Leg GSM ";
          txt2 = "rechts";
          printToLCD(txt1, txt2);
          reset = true;
          break;
        case 3:
          //Only right phone placed
          digitalWrite(LED_Left, LOW);
          digitalWrite(LED_Right, HIGH);
          txt1 = "Leg GSM ";
          txt2 = "links";
          printToLCD(txt1, txt2);
          reset = true;
          break;
        case 4:
          //Both phones are placed
          digitalWrite(LED_Left, HIGH);
          digitalWrite(LED_Right, HIGH);
          // Print a message to the LCD once 
          String str(subjects[subjectNumber]);
          int lengte = str.length();
          txt1 = str.substring(0,8);
          txt2 = str.substring(8,16);
          printToLCD(txt1, txt2);
          break;
      }
      
    }
    //else do nothing  
  }
  //Delay half a second to avoid rapid changes
  delay(500);
}

//Function to print to lcd
void printToLCD(String str1, String str2)
{
  lcd.home();          //back to start
  lcd.clear();         //clear screen
  lcd.print(str1);     //print left side
  lcd.setCursor(0,1);  //go to right
  lcd.print(str2);     //print right side
}
