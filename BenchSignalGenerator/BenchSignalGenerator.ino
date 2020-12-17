/* INSTRUCTIONS 
Set your RPM lowpoint (RPM_MIN) to the lowest point, do not 
go lower than 1000 RPM. Set your RPM highpoint (RPM_MAX) to 
the redline of your vehicle. The SPEED variable is used for 
the speed at which the generator sweeps through the RPM range. 
A higher SPEED variable will result in a slower speed. Try to keep 
this between (10 - 50). 


*/ 

float RPM_MIN = 1000; 
float RPM_MAX = 7200; 
int SPEED = 50; 

boolean accel = true; 
int currrpm; 

void setup()
{ 
  pinMode(5, OUTPUT); 
  currrpm = RPM_MIN; 
} 

int rpmconvert(int rpmin)
{
  return rpmin / 20.6;
}
void loop() 
{ 
  //tone(5, 300, 10);
  //delay(10);
  while (accel==false)
  { 
    currrpm += 50; 
    tone(5,rpmconvert(currrpm)); 
    delay(SPEED); 
    if (currrpm>RPM_MAX){accel=true;} 
  }
  while (accel==true)
  {
    currrpm -= 50; 
    tone(5,rpmconvert(currrpm)); 
    delay(SPEED); 
    if (currrpm<RPM_MIN){accel=false;} 
  } 
} 
