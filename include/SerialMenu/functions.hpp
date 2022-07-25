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

struct slaveSettings
{
	uint16_t Vmax;
	uint16_t Vfull;
	uint16_t Vlow;
	uint16_t Vmin;
	uint16_t VCal;

}slaveSettings;

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
bool selectID(int sID){

	if(sID<1)
	{Serial.println("Please select value above 0");return 0;}
	//make sure we have a active slave ID
	if(i2c_address[sID]!=1)
	{Serial.println("Please select a active ID");return 0;}
	Serial.println();
	return 1;
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

void changeID(byte nID){
	dataOut.command=0;
	dataOut.val1=0;
	dataOut.val2=0;
	dataOut.val3=0;
	if(nID<1)
	{Serial.println("Please select value above 0");return;}
	if(slaveID==nID)
	{Serial.println("Current ID selected\nPlease select a different ID");return;}
	//make sure we have a active slave ID
	if(i2c_address[nID]==1)
	{Serial.println("Selected ID already used\nPlease select a different ID");return;}
	Serial.println();
	dataOut.command=CHANGE_ID;
	dataOut.val1=nID;
	Wire.beginTransmission(slaveID);
	Wire.write((byte *) &dataOut,sizeof(dataOut));
	Wire.endTransmission();
	return;
}

void CurrentSettings(){
	dataOut.command=GET_SETTINGS;
	dataOut.val1=ON;
	Wire.beginTransmission(slaveID);
	Wire.write((byte *) &dataOut,sizeof(dataOut));
	Wire.endTransmission();
	delay(3);
	Wire.requestFrom(slaveID,10);
	slaveSettings.Vmax = ((Wire.read()<<8)|(Wire.read()&0xFF));
	slaveSettings.Vfull = ((Wire.read()<<8)|(Wire.read()&0xFF));
	slaveSettings.Vlow = ((Wire.read()<<8)|(Wire.read()&0xFF));
	slaveSettings.Vmin = ((Wire.read()<<8)|(Wire.read()&0xFF));
	slaveSettings.VCal = ((Wire.read()<<8)|(Wire.read()&0xFF));
	dataOut.val1=OFF;
	Wire.beginTransmission(slaveID);
	Wire.write((byte *) &dataOut,sizeof(dataOut));
	Wire.endTransmission();
	delay(3);
	Serial.print("Cell max: ");Serial.print(slaveSettings.Vmax);Serial.println("mV");
	Serial.print("Cell full: ");Serial.print(slaveSettings.Vfull);Serial.println("mV");
	Serial.print("Cell low: ");Serial.print(slaveSettings.Vlow);Serial.println("mV");
	Serial.print("Cell min: ");Serial.print(slaveSettings.Vmin);Serial.println("mV");
	Serial.print("Cell cal: ");Serial.print(slaveSettings.VCal);Serial.println("mV");
};