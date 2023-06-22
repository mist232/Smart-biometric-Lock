#include <ESP8266WiFi.h>
#include<NTPClient.h>
#include <WiFiUdp.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "T";           //WIFI SSID
const char* password = "mmea2327";            //WIFI PASSWORD

/* data received from application */
String  data = "";

int Relay4 = D4;     //D1
int BUZZER = D5;     //D1

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

void setup()
{
  /* initialize motor control pins as output */
  pinMode(Relay4, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(Relay4, HIGH);
  digitalWrite(BUZZER, LOW);

  /* start server communication */
  Serial.begin(115200);
  connectWiFi();
  server.begin();
  timeClient.begin();
  timeClient.setTimeOffset(19800);
}

void loop()
{
  /* If the server available, run the "checkClient" function */
  client = server.available();
  if (!client) return;
  data = checkClient ();
  Serial.print(data);
  /******** Run function according to incoming data from application *********

  if (data == "RELAY4SUCCESS")
  {
    digitalWrite(Relay4,LOW);
    digitalWrite(BUZZER, HIGH);
    delay(3000);
         timeClient.update();

  time_t epochTime = timeClient.getEpochTime();
  
  String formattedTime = timeClient.getFormattedTime();
  Serial.print("\nAccess Time: ");
  Serial.println(formattedTime);   

  String weekDay = weekDays[timeClient.getDay()];
  Serial.print("Week Day: ");
  Serial.println(weekDay);    

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);

  String currentMonthName = months[currentMonth-1];
  Serial.print("Month name: ");
  Serial.println(currentMonthName);

  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);
    digitalWrite(Relay4,HIGH);
    digitalWrite(BUZZER, LOW);
  }

}
//wifi connection establishing
void connectWiFi()
{
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
}
/************ RECEIVE DATA FROM the APP **************/
String checkClient (void)
{
  while (!client.available()) delay(1);
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length() - 9, 9);
  return request;
}
