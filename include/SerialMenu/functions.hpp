extern void detectI2C();
extern SerialMenu& menu;
extern byte i2c_address[];
struct dataOut
{
	byte command;
	byte val1;
	byte val2;
	byte val3;
}dataOut;

int selID;
int newID;
int slaveID=0;
const char noID[] PROGMEM = "No slave ID set\nPlease set slave ID in Main Menu";

void listI2C(){
	Serial.print("Actice slave addresses: ");
	for(unsigned int i=1;i<127;i++){
		if(i2c_address[i]==1){
		if(i%20==0)
		Serial.println();

		Serial.print(i);
		Serial.print(", ");
		}
	}
	Serial.println();
}
void reset(int sID){
	if(slaveID==0)
	{Serial.println(noID);return;}
	Wire.beginTransmission(sID);
	Wire.write(RESET_SETTINGS);
	Wire.endTransmission();
}
void restart(int sID){
	if(slaveID==0)
	{Serial.println(noID);return;}
	Wire.beginTransmission(sID);
	Wire.write(RESTART);
	Wire.endTransmission();
}

bool selectID(int sID){

	if(sID<1)
	{Serial.println("Please select value above 0");return 0;}
	//make sure we have a active slave ID
	if(i2c_address[sID]!=1)
	{Serial.println("Please select a active ID");return 0;}
	Serial.println();
	return 1;
}


bool changeID(byte oID,byte nID){

	if(nID<1)
	{Serial.println("Please select value above 0");return 0;}
	if(oID==nID)
	{Serial.println("Current ID selected\nPlease select a different ID");return 0;}
	//make sure we have a active slave ID
	if(i2c_address[nID]==1)
	{Serial.println("Selected ID already used\nPlease select a different ID");return 0;}
	Serial.println();
	dataOut.command=CHANGE_ID;
	dataOut.val1=nID;
	Wire.beginTransmission(oID);
	Wire.write((byte *) &dataOut,sizeof(dataOut));
	Wire.endTransmission();
	return 1;
}




extern void CurrentSettings(){
	dataOut.command=GET_SETTINGS;
	dataOut.val1=ALL;
	Wire.beginTransmission(slaveID);
	Wire.write((byte *) &dataOut,sizeof(dataOut));
	Wire.endTransmission();
	delay(5);

	

};
//extern void reset(int sID);
//extern void restart(int sID);