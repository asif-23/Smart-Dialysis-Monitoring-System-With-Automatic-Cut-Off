volatile int flow_frequency; // Measures flow sensor pulses
// Calculated litres/hour
 float vol = 0.0,l_minute;
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;




#include <LiquidCrystal.h>
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);
#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial mySerial(12, 3); 
 
const int DIA_RELAY=10;
const int BUZZER=11;
const int BUBBLE=A2;




int val=0;
int TEMPERATURE=0;
int count=0;
void flow () // Interrupt function
{
   flow_frequency++;
}

void setup() 
{

     


  Serial.begin(9600);
  mySerial.begin(9600);
  lcd.begin(16, 2);
  pinMode(BUZZER, OUTPUT);
  pinMode(DIA_RELAY, OUTPUT);
  pinMode(BUBBLE, INPUT);
  digitalWrite(BUZZER, LOW);
  digitalWrite(DIA_RELAY, LOW);
  

      lcd.setCursor(0, 0);
      lcd.print("IoTBASD DIALYSIS");
      lcd.setCursor(0, 1);
      lcd.print("MONITORING SYSTM"); 
      delay(5000); 
       lcd.setCursor(0, 0);
      lcd.print(" BOOTING MODEM  ");
      lcd.setCursor(0, 1);
      lcd.print(" PLEASE WAIT . ."); 
      delay(7000); 
      lcd.setCursor(0, 0);
      lcd.print("  MODEM READY!  ");
      lcd.setCursor(0, 1);
      lcd.print("                "); 
       delay(2000); 
       lcd.setCursor(0, 0);
       lcd.print("DIALYSIS PROCESS");
       lcd.setCursor(0, 1);
       lcd.print("    STARTED!!   ");
       digitalWrite(DIA_RELAY, HIGH);
       delay(2000);

       pinMode(flowsensor, INPUT);
   //digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
currentTime = millis();
   cloopTime = currentTime;

   
       lcd.clear();
}  

void loop() 
{

count+=1;
  if(count==1000)
  {
    count=0;
    pingserver();
  }









  currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0){
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      //lcd.clear();
      lcd.setCursor(14,1);
    //  lcd.print("Rate: ");
      lcd.print(l_minute);
      /*
      lcd.print(" L/M");
      l_minute = l_minute/60;
      lcd.setCursor(0,1);
      vol = vol +l_minute;
      lcd.print("Vol:");
      lcd.print(vol);
      lcd.print(" L");
      */
      flow_frequency = 0; // Reset Counter
      //Serial.print(l_minute, DEC); // Print litres/hour
      //Serial.println(" L/Sec");
    }
    else 
    {
      //Serial.println(" flow rate = 0 ");
      //lcd.clear();
      lcd.setCursor(14,1);
      //lcd.print("Rate: ");
      lcd.print( flow_frequency );
      lcd.print(" L/M");
      /*
      lcd.setCursor(0,1);
      lcd.print("Vol:");
      lcd.print(vol);
      lcd.print(" L");
      */
    }
   }








  
  val = analogRead(A0);
  float mv = ( val/1024.0)*5000; 
  TEMPERATURE = mv/10;


  lcd.setCursor(0, 0);
  lcd.print("TEMPERATURE:");
  lcd.print(TEMPERATURE);
  lcd.print("'F  ");
  if(digitalRead(BUBBLE)==LOW)
  {
  lcd.setCursor(0, 1);
  lcd.print("BUBBLE:NO");
  }
  lcd.print(" FLW:");
  
  if(digitalRead(BUBBLE)==HIGH)
  {
       lcd.setCursor(0, 0);
       lcd.print("ALERT! AIRBUBBLE");
       lcd.setCursor(0, 1);
       lcd.print("   DETECTED!!!  ");
       digitalWrite(BUZZER, HIGH);
       delay(3000);
       lcd.setCursor(0, 0);
       lcd.print("DIALYSIS PROCESS");
       lcd.setCursor(0, 1);
       lcd.print("  TERMINATED!!! ");
       digitalWrite(DIA_RELAY, LOW);

       sendbubblesms();
       digitalWrite(BUZZER, LOW);
       delay(3000);
       lcd.clear();
  }
  
  if(TEMPERATURE>100)
  {
       lcd.setCursor(0, 0);
       lcd.print("ALERT!!HIGH TEMP");
       lcd.setCursor(0, 1);
       lcd.print("VALUE DETECTED!!");
       digitalWrite(BUZZER, HIGH);
       delay(3000);
       lcd.setCursor(0, 0);
       lcd.print("DIALYSIS PROCESS");
       lcd.setCursor(0, 1);
       lcd.print("  TERMINATED!!! ");
       digitalWrite(DIA_RELAY, LOW);
       sendtempsms();
       digitalWrite(BUZZER, LOW);
       delay(3000);
       lcd.clear();
  }
  
  

  
  
  delay(10);
   
} 
void sendtempsms()
{
            lcd.setCursor(0, 0);
            lcd.print("HIGH TEMPERATURE");
            lcd.setCursor(0, 1);
            lcd.print(" SMS SENDING . .");
            
            delay(2000);
           
            lcd.setCursor(0, 0);
            lcd.print(" SMS SENDING TO ");
            lcd.setCursor(0, 1);
            lcd.print("REGISTERED PHNUM");
            delay(2000);   
            mySerial.println("AT");
            mySerial.write(13);
            mySerial.write(10); 
            delay(1000);
            mySerial.println("AT+CMGF=1");
            mySerial.write(13);
            mySerial.write(10); 
            delay(1000);
            mySerial.println("AT+CMGS=\"9847229676\"");
            mySerial.write(13);
            mySerial.write(10); 
            delay(1000);
            mySerial.println("ALERT!! HIGH TEMPERATURE DETECTED IN DIALYSIS PROCESS");
            mySerial.println("DIALYSIS INTERRUPTED");
            mySerial.println("https://iot-project24.000webhostapp.com/dia.html");
            mySerial.write(26);
            mySerial.write(10);
            delay(7000);
            lcd.setCursor(0, 0);
            lcd.print("    SMS SENT    ");
            lcd.setCursor(0, 1);
            lcd.print(" SUCCESSFULLY...");    
            delay(3000); 
              
}
void sendbubblesms()
{
            lcd.setCursor(0, 0);
            lcd.print("AIRBUBBLE DETCTD");
            lcd.setCursor(0, 1);
            lcd.print(" SMS SENDING . .");
            
            delay(2000);
           
            lcd.setCursor(0, 0);
            lcd.print(" SMS SENDING TO ");
            lcd.setCursor(0, 1);
            lcd.print("REGISTERED PHNUM");
            delay(2000);   
            mySerial.println("AT");
            mySerial.write(13);
            mySerial.write(10); 
            delay(1000);
            mySerial.println("AT+CMGF=1");
            mySerial.write(13);
            mySerial.write(10); 
            delay(1000);
            mySerial.println("AT+CMGS=\"9847229676\"");
            mySerial.write(13);
            mySerial.write(10); 
            delay(1000);
            mySerial.println("ALERT!! AIR BUBBLE DETECTED IN DIALYSIS PROCESS");
            mySerial.println("DIALYSIS INTERRUPTED");
            mySerial.write(26);
            mySerial.write(10);
            delay(7000);
            lcd.setCursor(0, 0);
            lcd.print("    SMS SENT    ");
            lcd.setCursor(0, 1);
            lcd.print(" SUCCESSFULLY...");    
            delay(3000); 
              
}
void pingserver()
{
  
   lcd.setCursor(0, 0);
        lcd.print("DATA LOGGING TO ");
        lcd.setCursor(0, 1);
        lcd.print("SERVER PLS WAIT "); 
        
   mySerial.println("AT");
  delay(1000);
  mySerial.println("AT+CPIN?");
  delay(1000);
  mySerial.println("AT+CREG?");
  delay(1000);
  mySerial.println("AT+CGATT?");
  delay(1000);
  mySerial.println("AT+CIPSHUT");
  delay(1000);
  mySerial.println("AT+CIPSTATUS");
  delay(2000);
  mySerial.println("AT+CIPMUX=0");
  delay(2000);
  mySerial.println("AT+CSTT=\"airtelgprs.com\"");//start task and setting the APN,
  delay(2000);
  mySerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
  mySerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
  mySerial.println("AT+CIPSPRT=0");
  delay(3000);
  mySerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000); 
  mySerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);    
  String str="GET http://api.thingspeak.com/update?api_key=A393SO49234IH20W&field5=" + String(TEMPERATURE) ;
  mySerial.println(str);//begin send data to remote server
  delay(4000);     
  mySerial.println((char)26);//sending
  delay(5000);
//waitting for reply, important! the time is base on the condition of internet 
  mySerial.println();
  mySerial.println("AT+CIPSHUT");//close the connection
  delay(100);

   lcd.setCursor(0, 0);
        lcd.print(" DATA LOADED TO ");
        lcd.setCursor(0, 1);
        lcd.print("WEB COMPLETED!! ");
        delay(2000);
        lcd.clear(); 
}
