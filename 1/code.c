
char aux_str[30];
char aux;

char latitude[15];
char longitude[15];

char inChar;  //used to decode gps
int index;
char inData[200];


void setup()
 {
   //Init the driver pins for GSM function
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
   //Output GSM Timing 
    digitalWrite(5,HIGH);
    delay(1500);
    digitalWrite(5,LOW); 
    
    Serial.begin(9600);
        // Use these commands instead of the hardware switch 'UART select' in order to enable each mode
    // If you want to use both GMS and GPS. enable the required one in your code and disable the other one for each access.
    digitalWrite(3,LOW);//enable GSM TX、RX
    digitalWrite(4,HIGH);//disable GPS TX、RX
    
    delay(20000); //wait 20secs for module to be ready
    
    start_GSM(); //initializes GSM/GPRS configs
    
    delay(5000); 
    
    start_GPS();  //initializates GPS 
  
    
 }
 void loop()    
 {    
   
   
   read_GPS();  //acquire actualLat/Long

   delay(2000);

   send_GPRS(); //send actual Lat/Long to desired url

   delay(30000);

 }
 
 void start_GSM(){
     //Configuracion GPRS Claro Argentina
    Serial.println("AT");
    delay(2000);
    Serial.println("AT CREG?");
    delay(2000);
    Serial.println("AT SAPBR=3,1,"APN","igprs.claro.com.ar"");  //replace with you mobile network APN
    delay(2000);
    Serial.println("AT SAPBR=3,1,"USER","clarogprs""); //replace with your  mobile network USER
    delay(2000);
    Serial.println("AT SAPBR=3,1,"PWD","clarogprs999""); //replace with your mobile password
    delay(2000);
    Serial.println("AT SAPBR=3,1,"Contype","GPRS"");  //config connection as GPRS
    delay(2000);
    Serial.println("AT SAPBR=1,1");  //set it
    delay(10000);
    Serial.println("AT HTTPINIT"); //initiate HTTP
    delay(2000);
    Serial.println("AT HTTPPARA="CID",1"); //mobile network CID
    delay(2000);
 }
 
 void send_GPRS(){  //here we send the actual Lat/long

          /*I adapted a PHP template to receive the lat/long data 
             if there is latitude=XXXXX&longitude=XXXX on header, PHP will write info into a TXT file
             If not, then, it will read TXT file and place a mark on a google map API
          **/


          Serial.print("AT HTTPPARA="URL","YOU.URL.COM/geo/geo2_0.php?latitude=");
          Serial.print(latitude);
          Serial.print("&longitude=");
          Serial.print(longitude);
          Serial.println(""");
          delay(2000);
          Serial.println("AT HTTPACTION=0"); //now GET action
          delay(2000);
          
          
 }
 
 
 void start_GPS(){
     //Configuracion en Inicializacion GPS
    Serial.print("AT");
    delay(1000);
    Serial.println("AT CGPSIPR=9600");// (set the baud rate)
    delay(1000);
    Serial.println("AT CGPSPWR=1"); // （turn on GPS power supply）
    delay(1000);
    Serial.println("AT CGPSRST=1"); //（reset GPS in autonomy mode）
    delay(120000); //delay long enough to assure GPS gets date, otherwise, location will be 0/0
 }
 
 void read_GPS(){
    
    
    Serial.println("AT CGPSINF=0");  //ask SIM908 GPS info
    
    read_String();  //read and store serial answer
    
    strtok(inData, ","); //decode string into comma separated values
    strcpy(longitude,strtok(NULL, ",")); // Gets longitude
    strcpy(latitude,strtok(NULL, ",")); // Gets latitude
    
    
    convert2Degrees(latitude);  //fix value 
    convert2Degrees(longitude); //fix value
    
    
 }
 
 
 void read_String() {
      index=0;
      
      while(Serial.available() > 0) // Don't read unless
                                                  // there you know there is data
   {
       if(index < 199) // One less than the size of the array
       {
           inChar = Serial.read(); // Read a character
           inData[index] = inChar; // Store it
           index  ; // Increment where to write next
           inData[index] = ''; // Null terminate the string
       }
   }
 }
 
 int8_t convert2Degrees(char* input){

    float deg;
    float minutes;
    boolean neg = false;    

    //auxiliar variable
    char aux[10];

    if (input[0] == '-')
    {
        neg = true;
        strcpy(aux, strtok(input 1, "."));

    }
    else
    {
        strcpy(aux, strtok(input, "."));
    }

    // convert string to integer and add it to final float variable
    deg = atof(aux);

    strcpy(aux, strtok(NULL, ''));
    minutes=atof(aux);
    minutes/=1000000;
    if (deg < 100)
    {
        minutes  = deg;
        deg = 0;
    }
    else
    {
        minutes  = int(deg) % 100;
        deg = int(deg) / 100;    
    }

    // add minutes to degrees 
    deg=deg minutes/60;


    if (neg == true)
    {
        deg*=-1.0;
    }

    neg = false;

    if( deg < 0 ){
        neg = true;
        deg*=-1;
    }
    
    float numeroFloat=deg; 
    int parteEntera[10];
    int cifra; 
    long numero=(long)numeroFloat;  
    int size=0;
    
    while(1){
        size=size 1;
        cifra=numero;
        numero=numero/10;
        parteEntera[size-1]=cifra; 
        if (numero==0){
            break;
        }
    }
   
    int indice=0;
    if( neg ){
        indice  ;
        input[0]='-';
    }
    for (int i=size-1; i >= 0; i--)
    {
        input[indice]=parteEntera[i] '0'; 
        indice  ;
    }

    input[indice]='.';
    indice  ;

    numeroFloat=(numeroFloat-(int)numeroFloat);
    for (int i=1; i<=6 ; i  )
    {
        numeroFloat=numeroFloat*10;
        cifra= (long)numeroFloat;          
        numeroFloat=numeroFloat-cifra;
        input[indice]=char(cifra) 48;
        indice  ;
    }
    input[indice]='';


}
 
