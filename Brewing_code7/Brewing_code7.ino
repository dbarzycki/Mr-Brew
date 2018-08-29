/* Created By 
   Daniel Barzycki
   Jed Bowen
   RIley Johns
   
   All you have to change is the mash time boiling time
   mash temp boiling temp and cooling temp
 */
double Mash_Time = 45;        // sets the mash time to "x" minutes
double Boiling_Time = 45;      // sets the boiling time to "x" minutes
double Mash_Temp = 150;        // Mash Temperature in °F
double Boiling_Temp = 193.2;    // Boiling temperature in °F
double Cooling_Temp = 88;     // Final Temperature in °F
double Cooling_Time = 30;     // Cooling time 

// Do not touch the rest of this code 

float Thermocouple = A5;    // thermocouple input pin
float Thermo = 0;
float temperature;
float tempf = 0;
float voltage = 0;
int Time = 500;    // sets the time delay to 0.5 second
unsigned long currentMillis = 0;
unsigned long startMillis;
float Push_Button = A3;
float push_button = 0;
float push_button_v = 0;
const int NPN_Transistor = 11;
const int Cooling_Relay = 10;
int x = 0;
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
float total = 0;
float average = 0;

void setup() 
{
   pinMode(Cooling_Relay, OUTPUT);     // sets the Cooling Relay pin as an output
   pinMode(NPN_Transistor,OUTPUT);    // set the NPN pin as an output
   Serial.begin(9600);   // sets the baud rate to 9600 the higher the number the faster it updates
   startMillis = millis();    // starts the timer
   for (int thisReading = 0; thisReading < numReadings; thisReading++){
      readings[thisReading] = 0;
   }
}

void loop(){
  
       push_button = analogRead(Push_Button);
       push_button_v = ((push_button*6.5)/1024);
       Serial.println(push_button_v);


       Thermo = analogRead(Thermocouple);
       voltage = ((Thermo*5)/1023);
       temperature = (voltage-1.25)/0.005;
       tempf = (temperature*1.8)+32 - (-391.55*voltage + 804.95);
       total = total - readings[readIndex];
       readings[readIndex] = tempf;
       total = total + readings[readIndex];
       readIndex = readIndex +1;
       if (readIndex >= numReadings) {
        readIndex = 0;
       }
       average = total / numReadings;
       Serial.print("Temp       °F ");
       Serial.println(average, 1);
       Serial.println(voltage, 3);

       delay (Time);
       
  if (push_button_v >= 2.02){
      Serial.println("on");

      while (x < 1){
        currentMillis = millis();
        Serial.println(currentMillis);
        x++;
      }
      
      if (millis() < ((Mash_Time*60*1000)+ currentMillis)){   // Mashing Time and temperature control loop

       Serial.print("Temp       °F ");
       Serial.println(average, 1);
       Serial.println(voltage, 3);
       
          if (average < (Mash_Temp - 0.35)){
            
            digitalWrite(NPN_Transistor, HIGH);
          }

          if (average > (Mash_Temp -0.2 )){
            
            digitalWrite(NPN_Transistor, LOW);
          }
    }

    if (millis() > ((Mash_Time*60*1000)+ currentMillis) && millis() < (Mash_Time*60*1000)+ currentMillis + (Boiling_Time*60*1000)){   // Boiling time and temperature control loop

       Serial.print("Temp       °F ");
       Serial.println(average, 1);
          
          if (average < (Boiling_Temp - .1)){
            
            digitalWrite(NPN_Transistor, HIGH);
          }

          if (average > (Boiling_Temp -0.05 )){
            
            digitalWrite(NPN_Transistor, LOW);
          }
    }

    if ((millis() > (((Mash_Time*60*1000)+ currentMillis + (Boiling_Time*60*1000)))) && millis() < ((Mash_Time*60*1000)+ currentMillis + (Boiling_Time*60*1000)+ Cooling_Time*60*1000)){   // Cooling time and temperature control loop

       Serial.print("Temp       °F ");
       Serial.println(average, 1);

          if (average > (Cooling_Temp + 2)){
            
            digitalWrite(NPN_Transistor, LOW);
            digitalWrite(Cooling_Relay, HIGH);
            Serial.print("Cooling coil on");
          }

          if (average < (Cooling_Temp - 2 )){
            
            digitalWrite(Cooling_Relay, LOW);
            Serial.print("Cooling Coil off");
            Serial.println(average , 1);
          }
    }
  }
  if (push_button_v <2.02){
       
    Serial.println("off");
    digitalWrite(NPN_Transistor, LOW);
    digitalWrite(Cooling_Relay, LOW);
    Serial.println(average , 1);
  }

 delay(Time);
    
}
