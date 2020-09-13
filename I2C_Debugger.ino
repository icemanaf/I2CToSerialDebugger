#include <Wire.h>
#include <string.h>

void setup() {
  // put your setup code here, to run once:

  Serial.begin(500000);
  
  delay(500);
  
  Wire.begin(4, 5); //4 and 5 are the GPIO pins of the esp8266 D1->CLK D2->DATA
  
  delay(500);
  
}

void ScanI2C()
{
  byte error, address;
  int nDevices;
 
  nDevices = 0;

  Serial.println("STARTSCAN");
  
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      if (address>=16)
        Serial.println(address);
      nDevices++;
    }
      
  }

   Serial.print("ENDSCAN,");
   Serial.print(nDevices);
}


void ReadSerial(char *buffer)
{
  int counter=0;
  char chr[2];
  chr[1]='\0';
  strcpy(buffer,"");
  while(true)
  {
      if (Serial.available())
      {
        chr[0]=Serial.read();
        if (chr[0]=='\r')
        {
          break;
        }
        if (chr[0]!='\n')
        {
          strcat(buffer,chr);
          counter++;
        }
        if (counter>20)
        {
          break;  
        }
      }
  }
}

void Read()
{
   char  input[20];
   int addr,reg,value,counter,ret;

    Serial.println("addr?");
    ReadSerial(input);
    addr=atoi(input);
    
    Serial.println("reg?");
    ReadSerial(input);
    reg=atoi(input);
    
    Serial.println("val?");
    ReadSerial(input);
    value=atoi(input);

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.beginTransmission(addr);
    Wire.requestFrom(addr,value);

    for(counter=0;counter<value;counter++)
    {
      ret=Wire.read();
      Serial.println(ret);
    }

    ret=Wire.endTransmission();

    Serial.print("END,");
    Serial.print(ret);
    Serial.print(",");
    Serial.print("READ,");
    Serial.print(addr);
    Serial.print(",");
    Serial.print(reg);
    Serial.print(",");
    Serial.print(value);
    
}


void Write()
{
    char  input[20];
    int addr,reg,value,ret;

    Serial.println("addr?");
    ReadSerial(input);
    addr=atoi(input);
    
    Serial.println("reg?");
    ReadSerial(input);
    reg=atoi(input);

    Wire.beginTransmission(addr);
    Wire.write(reg);

    while(true)
    {
      Serial.println("val?");
      ReadSerial(input);

      if (strstr(input,"END")) break;
      
      value=atoi(input);
      Wire.write(value);
    }
    Wire.endTransmission();

    Serial.print("END,");
    Serial.print(ret);
    Serial.print(",");
    Serial.print("WRITE,");
    Serial.print(addr);
    Serial.print(",");
    Serial.print(reg);
    Serial.print(",");
    Serial.print(value);
}


void loop()
{
  // put your main code here, to run repeatedly:

    char  input[20];
  
    ReadSerial(input);

    if (strstr(input,"SCAN"))
    {
       ScanI2C();
    }
    else if (strstr(input,"READ"))
    {
       Read();
    }
    else if(strstr(input,"WRITE")){
      Write();
    }

    Serial.println("");
  }
 
