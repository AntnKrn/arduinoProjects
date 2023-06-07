#include <Wire.h>          
#include <LiquidCrystal_I2C.h>  
LiquidCrystal_I2C layar(0x27, 16, 2);
#define DS1621_ADDRESS  0x48

const int** D = 11;
const int** S = 10;
const int** C = 13;
int counter = 0;
unsigned long prevMillis = 0;
unsigned long interval = 3000;

void setup() {
  layar.begin(A1, A2);
  layar.backlight();   
  layar.setCursor(6, 0);  
  Wire.beginTransmission(DS1621_ADDRESS); // connect to DS1621 (send DS1621 address)
  Wire.write(0xAC);                       // send configuration register address (Access Config)
  Wire.write(0);                          // perform continuous conversion
  Wire.beginTransmission(DS1621_ADDRESS); // send repeated start condition
  Wire.write(0xEE);                       // send start temperature conversion command
  Wire.endTransmission();                 // stop transmission and release the I2C bus




  pinMode(S, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(C, OUTPUT);
  digitalWrite(S, HIGH);

 delay(50);
  WRENtoM45PE40();
  pageErase();
  for(int i = 0; i < 24; i++){
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  }
  delay(50);
  WRDI();    
}
void loop() {
  delay(1000);
  int temp = get_temperature() / 10 ;
  layar.clear();    
  layar.print(temp);
  unsigned long currentMillis = millis();

  if(currentMillis - prevMillis >= interval){
    writeTempPerMin(temp);
    prevMillis = currentMillis;
  }


}

void writeTempPerMin(int temp){
  int sizeOfArr = length(temp);
  WRENtoM45PE40();
  PW();
  setAddress();
  toASCII(sizeOfArr, temp);
  digitalWrite(S, HIGH);
  delay(50);
  WRDI(); 
}
void toASCII(int length, int temp2){
  if(temp2 >= 0){
    int arr[length];
    for(int i = length - 1; i >= 0; i--){
      arr[i] =  temp2 % 10;
      temp2 /= 10;
    }
    for(int i = 0; i < length; i++){
      digitalWrite(C, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(C, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(C, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      int binNum[4];
      int index = 0;
      while(index < 4){
        binNum[index] = arr[i] % 2;
        arr[i] /= 2;
        index++;
      }
      for(int j = index - 1; j >= 0; j--){
        if(binNum[j] == 0){
          digitalWrite(C, HIGH);
          digitalWrite(C, LOW);
        }
        if(binNum[j] == 1){
          digitalWrite(D, HIGH);
          digitalWrite(C, HIGH);
          digitalWrite(C, LOW);
          digitalWrite(D, LOW);
        }
      }
    }
  }
  if(temp2 < 0){
    temp2 = temp2 * (-1);
    int arr[length];
    for(int i = length - 1; i >= 0; i--){
      arr[i] =  temp2 % 10;
      temp2 /= 10;
    }
    digitalWrite(C,HIGH);
    digitalWrite(C,LOW);
    digitalWrite(C,HIGH);
    digitalWrite(C,LOW);
    digitalWrite(D,HIGH);
    digitalWrite(C,HIGH);
    digitalWrite(C,LOW);
    digitalWrite(D,LOW);
    digitalWrite(C,HIGH);
    digitalWrite(C,LOW);
    digitalWrite(D,HIGH);
    digitalWrite(C,HIGH);
    digitalWrite(C,LOW);
    digitalWrite(C,HIGH);
    digitalWrite(C,LOW);
    digitalWrite(D,LOW);
    digitalWrite(C,HIGH);
    digitalWrite(C,LOW);
    digitalWrite(D,HIGH);
    digitalWrite(C,HIGH);
    digitalWrite(C,LOW);
    digitalWrite(D,LOW);
    for(int i = 0; i < length; i++){
      digitalWrite(C, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(C, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(C, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      int binNum[4];
      int index = 0;
      while(index < 4){
        binNum[index] = arr[i] % 2;
        arr[i] /= 2;
        index++;
      }
      for(int j = index - 1; j >= 0; j--){
        if(binNum[j] == 0){
          digitalWrite(C, HIGH);
          digitalWrite(C, LOW);
        }
        if(binNum[j] == 1){
          digitalWrite(D, HIGH);
          digitalWrite(C, HIGH);
          digitalWrite(C, LOW);
          digitalWrite(D, LOW);
        }
      }
    }
  }
}

int length(int num) {
  int x = 1;
  if(num > 0){
    while ((num/=10) > 0) x++;
    return x;
  } 
  if(num <= 0){
    num = num * (-1);
    while ((num/=10) > 0) x++;
    return x;
  }
}

int16_t get_temperature() {
  Wire.beginTransmission(DS1621_ADDRESS); // connect to DS1621 (send DS1621 address)
  Wire.write(0xAA);                       // read temperature command
  Wire.endTransmission(false);            // send repeated start condition
  Wire.requestFrom(DS1621_ADDRESS, 2);    // request 2 bytes from DS1621 and release I2C bus at end of reading
  uint8_t t_msb = Wire.read();            // read temperature MSB register
  uint8_t t_lsb = Wire.read();            // read temperature LSB register
 
  int16_t raw_t = (int8_t)t_msb << 1 | t_lsb >> 7;
  raw_t = raw_t * 10/2;
  return raw_t;
}




void setAddress(){
  for(int i = 0; i < 8; i++){
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  }
  for(int i = 0; i < 8; i++){
    digitalWrite(C, HIGH);
    digitalWrite(C, LOW);
  }
  toBinary(counter);

}

void toBinary(int number){
  int binNum[8];
  int index = 0;
  while(index < 8){
    binNum[index] = number % 2;
    number /= 2;
    index++;
  }

  for(int i = index - 1; i >= 0; i--){
    if(binNum[i] == 0){
      digitalWrite(C, HIGH);
      digitalWrite(C, LOW);
    }
    if(binNum[i] == 1){
      digitalWrite(D, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
    }
  }
  counter++;
  counter++;
  counter++;
  counter++;
}

void sectorErase() {
  digitalWrite(S, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
}

void WRENtoM45PE40(){
  digitalWrite(S, LOW);
  digitalWrite(D, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(S, HIGH);
}

void WRDI(){
  digitalWrite(S, LOW);
  digitalWrite(D, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(S, HIGH);
}

void PW(){
  digitalWrite(S, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);

}



void pageErase(){
  digitalWrite(S, LOW);

  digitalWrite(D, HIGH);

  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);

  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);

  digitalWrite(D, LOW);

  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);

  digitalWrite(D, HIGH);

  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);  

  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);

  digitalWrite(D, LOW);

  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);

  digitalWrite(D, HIGH);

  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);  

  digitalWrite(C, HIGH);
  digitalWrite(C, LOW);

  digitalWrite(D, LOW);
}
