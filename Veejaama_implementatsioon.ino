
#define FLOWSENSORPIN 12                                            
volatile uint16_t pulses = 0;                                       
volatile uint8_t lastFlowPinState;                                  
volatile uint32_t lastFlowRateTimer = 0;                            
volatile float flowrate;                                            
int turbidityAnalogIn = 0;                                          
int turbidityDigitalIn = 3;                                         

int liquidLevel = 0;                                                
int liquidLevelSignalPin = 2;     
                                  
SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN);
                                          
  if (x == lastFlowPinState) {
    lastFlowRateTimer++;
    return;                                                         
  }
  
  if (x == HIGH) {
    pulses++;                                                      
  }
  
  lastFlowPinState = x;
  flowrate = 1000.0;
  flowrate /= lastFlowRateTimer;                                    
}

void useInterrupt(boolean v) {
  if (v) {
    OCR0A = 0xAF; 
    TIMSK0 |= _BV(OCIE0A);
  } else {
    TIMSK0 &= ~_BV(OCIE0A);                                         
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(FLOWSENSORPIN, INPUT);                                    
  digitalWrite(FLOWSENSORPIN, HIGH);                                
  pinMode(turbidityDigitalIn, INPUT);                               
  pinMode(liquidLevelSignalPin,INPUT);                              
  lastFlowPinState = digitalRead(FLOWSENSORPIN);                    
  useInterrupt(true);
}

void loop() {
  Serial.print("Sagedus: ");
  Serial.print(flowrate);
  Serial.println(" Hz");
  Serial.print("Impulsid: ");
  Serial.println(pulses, DEC);

  
  float liters = pulses;
  liters /= 7.5;
  liters /= 60.0;
  liters /= 10;                                                    

  Serial.print(liters);                                             
  Serial.println(" Liters");

  int turbidityAnalogValue = analogRead(turbidityAnalogIn);         
  float voltage = turbidityAnalogValue * (5.0 / 1024.0);            
  Serial.print("Turbidity analog: ");                                
  Serial.println(voltage);                                          
  Serial.print("Turbidity digital: ");                              
  Serial.println(digitalRead(turbidityDigitalIn));                  

  liquidLevel = digitalRead(liquidLevelSignalPin);                  
  Serial.print("Vedeliku tase = ");                                  
  Serial.println(liquidLevel, DEC);                                   
  
  delay(5000);
}
