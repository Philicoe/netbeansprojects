//Payfare Bubbly 2016 - Vega 5000S
//Loads 10 card credits and checks card ballance, and shows totals

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctosapi.h>
#include <ctos_clapi.h>

//
//
//
void PrintStringCenter(BYTE y,STR *sBuf);
void PrintStringCenter2lINES(BYTE y,BYTE y2 ,STR *sBuf,STR *sBuf2);
BOOL doCollect(STR heading[],int positionX,int positionY,int maxChar,STR output[],STR initial[],BOOL pwd);
void getSerial(void);
void setTimeDate(void);
void getTimeDate(void);
void showError(STR comment[]);
int StrToInt(STR *sStr);
ULONG PowTen(USHORT usP);
BOOL pollSetup(BYTE display[]);
BOOL pollNetwork(void);
BOOL openGprs(void);
BOOL closeGprs(void);
STR intToAscii(int input,STR output[],int length);
void testData(STR qData[]);
BYTE checkBattery(void);
void setLCD(void);
BOOL recreate(BYTE *fileName);
BOOL stomem(BYTE *fileName,BYTE *data,int position,BOOL begin);
BOOL getmem(BYTE *fileName,BYTE data[],ULONG dataLen,int position,BOOL begin);
BOOL stoMerchant(STR dispMerch[]);
BOOL getMerchant(STR dispMerch[]);
int getTotalP(void);
int getTotalC(void);
BOOL stoTotalP(int total);
BOOL stoTotalC(int total);
void doUploadType(void);
void doUploadMode(void);
BOOL stoSettings(void);
BOOL getSettings(void);
BOOL deleteTran(void);
void doTotals(void);
void doTotalsScrn(void);
void doReport(void);
BOOL stoTran(STR tType[],STR tRef[],STR tGross[],STR tDisc[],STR tTip[],STR tNett[],STR tBal[],STR tSer0[],STR tSer1[],STR tId0[],STR tId1[],STR tPurse[],STR tLpoints[],STR tLbal[],BOOL sent);
BOOL addToTotalP(int amount);
BOOL addToTotalC(int amount);
BOOL getTran(int offset, STR tranData[]);
BOOL uploadTran(void);
int getTranCountAll(void);
int getTranCountSent(void);
int getTranCountPend(void);
int StartTrans(ULONG ulValue,STR cardNumberX[],STR cardID[],STR low[]);
BOOL balsto(BYTE *fileName,BYTE *data);
BOOL balMake(BYTE *fileName);
BOOL balExist(BYTE *fileName);
int balGet(BYTE *fileName,BYTE *data);
ULONG setTags(void);
ULONG setAID(void);
ULONG setCAPK1(void);
ULONG setCAPK2(void);
ULONG setCAPK3(void);
ULONG setParam(void);
unsigned short TLV_Get_Value(unsigned char *buf, unsigned long *tag_value, unsigned short *len_value);
BOOL onCradle(void);
void doPrint(STR xName[],STR xNumber[],STR xStoreName[],STR xCentre[],STR xAmount[],STR xBalance[],BOOL client);
STR scrnFormat(STR input[],STR output[],int length,BOOL decimal,BOOL blank);
void packSub(int offset, int length, STR in[], STR out[]);
BOOL getTag(STR in[],STR tag[], STR out[]);
BOOL getConfig(void);
BOOL fetchConfig(void);
BOOL stoConfig(void);
BOOL deleteConf(void);
BOOL Check_Card_state(BYTE *ATQA_f, BYTE *SAK_f, BYTE *SN, BYTE *SN_Len);
BOOL Authenticate(BYTE Block, BYTE KeyType, BYTE Key[6], BYTE *sn);
int getPlace(BYTE KeyX[],BYTE csnX[]);
int stoPlace(BYTE KeyX[],BYTE csnX[],int sto);
BOOL writeLog(BYTE KeyX[],BYTE csnX[],int counter,BYTE word1[],BYTE word2[]);
BOOL doLog(STR cardIDZ[],STR balZX[]);
int getText(STR output[],int maxLength,int type,STR legend[]);
BOOL isFirstTimeConfig=TRUE;


STR sCountry[5];
STR sSystem[5];
STR sClient[5];
STR sKey[5];
STR sLoyalty[3];
STR sCardType[2];
STR sTermType[2];
STR sUpload[2];
STR sCrDr[2];
BOOL sRcptA;
BOOL sRcptB;
BOOL sEnable;
BOOL sMerchCard;
STR sHost[30];
STR sSchema[30];
STR sTableHdr[30];
STR sClientName[30];
STR sMerchantName[30];
STR sDisp[20];
STR sDispoverflow[20];
STR configDb[60];
BOOL bDisplayIsOverFlowing;
BOOL doUpload;
BOOL gprsLan;
BYTE iSocket;
USHORT usrtn = d_OK;

/* Card key */
BYTE currentCardKey[2]={0x08,0x08};

BYTE bStr[30];
BYTE baLocalIP[4] = {0};
BYTE baRemotelIP[4] ={ 197 , 242 , 146 , 179};
USHORT usPort = 10042;
char APN[20] = "internet";
BYTE lanMask[] = "255.255.255.0";
BYTE lanHostIp[] = "10.0.0.50";//"197.242.146.179";
BYTE lanHostPort[] = "10042";
STR dbIp[] = "10.0.0.50";//"197.242.146.179";"197.242.146.179";//"localhost";
USHORT usSendLen;
USHORT usRecvLen;
BYTE rxData[512];
BYTE rxDataX[512];
BYTE rxDataZ[512];
BYTE msgXml[512];
BOOL online;
int i,ii,j,k,amount,resp;
BYTE baBuf[200];
BYTE sBuf[20];
BYTE key;
STR termSerial[16];
STR termSerialFinal [22];
STR merchantIDX[4];

STR dispTime[6];
STR dispDate[9];
STR tranTime[9];
STR tranDate[9];
STR expiryDate[5];
STR dispTD[17];
STR merchant[17];
BYTE baDateTime[20];
STR tranType[3];
STR cardNumber[17];
STR cardName[17];
STR cardID[14];
//
//
//
//Main Entry Point
//
int main(int argc,char *argv[])
{
    BYTE battery[3];
    BYTE display[20];
    STR bDisp[17];
    STR temp1[3000];
    BYTE bb[1];
    STR keyy[3];
    
     STR cardBalance[15];
    STR cardAmount[15];
    STR setPin[5];
    int retCard,cp;
    STR balDisp[17];
    STR balX[15];
    STR balZ[15];
    STR AmtX[15];
    STR AmtZ[15];
  
    USHORT ret;
    BYTE buff[20];
    BYTE baBuff[128];
    ULONG retret;
    	BYTE baRBuf1[2000];
	UINT uiRLen1;
     	int temp,err;
	STR balanceXML[11];	
	STR amountXML[11];	
	STR tempDisp[17];
	STR tempInt[2];
        BOOL cancel;
        	STR zAmount[20];
STR Balance[16];
STR Total[16];
STR TotalX[16];
STR Amount[16];
STR dispAmtX[16];
STR tempX[16];
STR lowb[16];
STR type[3];

int loyaltyPercent;
int loyaltyBalane;
strcpy(sHost,"10.0.0.50");//"197.242.146.179";);
strcpy(sSchema,"bubbly2017");
    setLCD();
    getSettings();
    strcpy(configDb,"|10.0.0.50|bubbly2017|termsetup|payfare|p4yf4r3|");   //Changes made here from |10.0.0.50|payfare|termsetup|payfare|p4yf4r3| to... |10.0.0.50|bubbly2017|termsetup|payfare|p4yf4r3|
    CTOS_LCDTClearDisplay();
    PrintStringCenter(3,"Please Wait ..");
    CTOS_CLInitComm(d_COMM_DEFAULT_BAUDRATE);
    online=FALSE;
    bDisplayIsOverFlowing=FALSE;
    if(gprsLan==TRUE)
    {
        CTOS_TCP_GPRSInit();
        CTOS_TCP_SetConnectTO(5000);
    }
    else
    {
        CTOS_EthernetEnable ();
        memset(buff, 0, sizeof(buff));
        memcpy(buff, (BYTE*)lanHostIp,strlen(lanHostIp));
        ret = CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_HOSTIP, buff, strlen(buff));
        memset(buff, 0, sizeof(buff));
        memcpy(buff, (BYTE*)lanHostPort,strlen(lanHostPort));
        ret = CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_HOSTPORT, buff, strlen(buff));
        memset(buff, 0, sizeof(buff));
        memset(buff, 0, sizeof(buff));
        buff[0] = 0x32;
        ret = CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_AUTOCON, buff, 1);
        buff[0] = 0x31;
        ret = CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_DHCP, buff, 1);
        ret = CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_UPDATE_EXIT, baBuff, 0);
        if(ret != d_OK)
        {
            showError("Lan Fail!");
        }
        CTOS_Delay(1000);
    }
    
    getSerial();
    getConfig();
    loyaltyPercent=StrToInt(sLoyalty);
    if(doUpload==TRUE)
    {
        if(gprsLan==TRUE)
        {
            PrintStringCenter(3,"Opening GPRS ...");
        }
        else
        {
            PrintStringCenter(3,"Opening LAN ...");
        }
        if(openGprs())
        {
            showError("Connection Open");
        }
        else
        {
            showError("Connection FAIL");
        }   
    } 
    
    restart:
    getConfig();
    bb[0]=checkBattery();
    if(bb[0]>0)
    {
        int bc=bb[0];
        intToAscii(bc,battery,3);
         if(battery[0]=='0')
        {
            battery[0]=' ';
            if(battery[1]=='0')
                battery[1]=' ';
        }
        CTOS_LCDTClearDisplay();
        strcpy(bDisp,"Battery: ");
        strcat(bDisp,battery);
        strcat(bDisp,"%");
       
        PrintStringCenter(3,bDisp);
        CTOS_Delay(500);
    }
    getSerial();
    CTOS_LCDTClearDisplay();
    
    menu0:
    
    CTOS_TimeOutSet ( TIMER_ID_1, 60000 );
    
    menu:
    
    CTOS_LCDForeGndColor(0x0000ff);
    if(bDisplayIsOverFlowing==TRUE)
    {
        //CTOS_LCDTPrintXY(1,2,sDisp);
        //CTOS_LCDTPrintXY(1,3,sDispoverflow);
        PrintStringCenter2lINES(2,3,sDisp,sDispoverflow);
    }else
    {        
       PrintStringCenter(3,sDisp);
    }
    
   // PrintStringCenter(3,sDisp);
    CTOS_LCDForeGndColor(0x000000);
    
    menu1:
    
    menu2:
    
    getTimeDate();
    CTOS_LCDTPrintXY(1,1,dispTD);
    if(sCrDr[0]=='D')
    {
        CTOS_LCDTPrintXY(1,5,"1. Card Sales  ");
    }
    else
    {
        CTOS_LCDTPrintXY(1,5,"1. Card Credits ");
    }   
    CTOS_LCDTPrintXY(1,6,"2. Card Balance ");
    CTOS_LCDTPrintXY(1,8,"3. Totals       ");
    if(online==TRUE)
        CTOS_LCDTPrintXY(16,8,"c");
    else
        CTOS_LCDTPrintXY(16,8," ");
   
        
    if(CTOS_KBDHit(&key)==d_OK)
    {
        if(key=='1')
        {
                goto sale;
        }
        if(key=='2')
        {
                goto enquire;
        }
        if(key=='3')
        {
            CTOS_LCDTClearDisplay();
            doTotalsScrn();
            goto restart;
        }
        if(key=='0')
        {
            goto manager1;
        }
     }
    if((doUpload == TRUE)&&(online==TRUE))
    {
         if (CTOS_TimeOutCheck(TIMER_ID_1)==d_YES)
         {
             CTOS_LCDTClearDisplay();
             PrintStringCenter(3,"Polling Network");
             if(pollNetwork())
                 online=TRUE;
	     else
                 online=FALSE;
         }
         goto restart;
     }        
    goto menu1;
        
    manager:
    
    CTOS_LCDTClearDisplay();
    if(!doCollect("PASSWORD",7,3,4,setPin,"    ",TRUE))
        goto restart;
    if((setPin[0]!='2')||(setPin[1]!='0')||(setPin[2]!='0')||(setPin[3]!='7'))
        goto restart;
    goto sysctrl;
    
    manager1:
    
    CTOS_LCDTClearDisplay();
    PrintStringCenter(1,"Manager Mode");
    CTOS_LCDTPrintXY(1,3,"1. Set Time/Date");
    CTOS_LCDTPrintXY(1,4,"2. Upload");
    CTOS_LCDTPrintXY(1,5,"3. Print Totals");
    CTOS_LCDTPrintXY(1,6,"4. Print Report");
    CTOS_LCDTPrintXY(1,7,"5. Power Off");
    CTOS_LCDForeGndColor(0x0000ff);
    CTOS_LCDTPrintXY(9, 8, "[X]");
    CTOS_LCDForeGndColor(0x000000);
    CTOS_LCDTPrintXY(13, 8, "Exit");
    while(1)
    {
        CTOS_KBDGet(&key);
        if(key==d_KBD_CANCEL)
            goto restart;
        if((key>='0')&&(key<='5'))
                break;
  }
    if(key=='0')
    {
        goto manager;
    }
    if(key=='1')
    {
        setTimeDate();
        goto manager1;
    }
    if(key=='2')
    {
        CTOS_LCDTClearDisplay();
        cp=getTranCountPend();
        if(cp>0)
        {
            if(!online)
            {
                CTOS_LCDTClearDisplay();
                PrintStringCenter(3,"System Offline");
                PrintStringCenter(5,"Connect?");
                CTOS_LCDForeGndColor(0x0000ff);
                CTOS_LCDTPrintXY(9,8,"[X]");
                CTOS_LCDForeGndColor(0x21cd3a);
                CTOS_LCDTPrintXY(13,8,"[OK]");
                CTOS_LCDForeGndColor(0x000000);
                while(1)
                {
                        CTOS_KBDGet(&key);
                        if(key==d_KBD_CANCEL)
                                goto manager1;
                        if(key==d_KBD_ENTER)
                                break;
                }
                CTOS_LCDTClearDisplay();
                if(gprsLan==TRUE)
                    PrintStringCenter(3,"Opening GPRS ...");
                else
                    PrintStringCenter(3,"Opening LAN ...");
                if(openGprs())
                {
                    CTOS_LCDTClearDisplay();
                    PrintStringCenter(3,"Uploading");
                    if(uploadTran())
                    {
                        showError("Upload OK");
                    }
                    else
                    {
                        showError("Upload Failure");
                    }
                    if(gprsLan==TRUE)
                        showError("Closing GPRS ...");
                    else
                        showError("Closing LAN ...");
                    closeGprs();
                }
                else
                {
                        showError("Comm Fail !");
                }
            }
            else
            {
                CTOS_LCDTClearDisplay();
                PrintStringCenter(3,"Uploading");
                if(uploadTran())
                        showError("Upload OK");
                 else
                        showError("Upload Failure");
                goto manager1;
             }
        }
        else
        {
             showError("No Transactions!");
        }
        goto manager1;
    }
    if(key=='3')
    {
         CTOS_LCDTClearDisplay();
         PrintStringCenter(3,"Printing");
         PrintStringCenter(4,"Please Wait ..");
         doTotals();
         goto manager1;
    }
    if(key=='4')
    {
         CTOS_LCDTClearDisplay();
         PrintStringCenter(1,"REPORTS");
         sprintf(tempX,"%1d",getTranCountAll());
         CTOS_LCDTPrintXY(1,3,"All Sales");
         CTOS_LCDTPrintXY(14,3,tempX);
         sprintf(tempX,"%1d",getTranCountPend());
         CTOS_LCDTPrintXY(1,4,"Pend Sales");
         CTOS_LCDTPrintXY(14,4,tempX);
         sprintf(tempX,"%1d",getTranCountSent());
         CTOS_LCDTPrintXY(1,5,"Sent Sales");
         CTOS_LCDTPrintXY(14,5,tempX);
         CTOS_LCDTPrintXY(1,8,"                ");
         CTOS_LCDTPrintXY(1,8,"Print?");
         CTOS_LCDForeGndColor(0x0000ff);
         CTOS_LCDTPrintXY(9,8,"[X]");
         CTOS_LCDForeGndColor(0x21cd3a);
         CTOS_LCDTPrintXY(13,8,"[OK]");
         CTOS_LCDForeGndColor(0x000000);
         while(1)
         {
             CTOS_KBDGet(&key);
             if(key==d_KBD_CANCEL)
                 goto manager1;
             if(key==d_KBD_ENTER)
                 break;
         }
         PrintStringCenter(3,"Printing");
         PrintStringCenter(4,"Please Wait ..");
         doReport();
         goto manager1;
    }
    if(key=='5')
    {
            CTOS_LCDTClearDisplay();
            PrintStringCenter(3,"Powering Off");
            CTOS_PowerOff();
            exit(0);
    }
    goto manager1;
    
    sysctrl:
    
     CTOS_LCDTClearDisplay();
    PrintStringCenter(1,"System Control");
    CTOS_LCDTPrintXY(1,2,"1. Gprs/Lan");
    CTOS_LCDTPrintXY(1,3,"2. Upload Mode");
    CTOS_LCDTPrintXY(1,4,"3. Get Config");
    CTOS_LCDTPrintXY(1,5,"4. Clear Sales");
    CTOS_LCDTPrintXY(1,6,"5. Exit Program");
    CTOS_LCDTPrintXY(1,7,"6. Power Off");
    CTOS_LCDForeGndColor(0x0000ff);
    CTOS_LCDTPrintXY(9, 8, "[X]");
    CTOS_LCDForeGndColor(0x000000);
    CTOS_LCDTPrintXY(13, 8, "Exit");
    while(1)
    {
        CTOS_KBDGet(&key);
        if(key==d_KBD_CANCEL)
            goto restart;
        if((key>='1')&&(key<='8'))
                break;
    }
    if(key=='1')
    {
        doUploadType();
        goto sysctrl;
    }
    if(key=='2')
    {
        doUploadMode();       
        goto sysctrl;
    }
    if(key=='3')
    {
    
     if(!isFirstTimeConfig)
         goto getConfigstart;
             
     getconfigChoice:
        CTOS_LCDTClearDisplay();
        PrintStringCenter(1,"First Time Conf?");
        CTOS_LCDTPrintXY(1,2,"1. Yes");
        CTOS_LCDTPrintXY(1,3,"2. No");
        CTOS_LCDForeGndColor(0x0000ff);
        CTOS_LCDTPrintXY(9, 8, "[X]");
        CTOS_LCDForeGndColor(0x000000);
        CTOS_LCDTPrintXY(13, 8, "Exit");
        while(1)
        {
            CTOS_KBDGet(&key);
            if(key==d_KBD_CANCEL)
                goto sysctrl;
            if((key>='1')&&(key<='8'))
                    break;
        }
        if(key=='1')
        {

            goto getMerchantIDX; // get merch idx to get original pos conf without DB knowing POS serialnumber
        }
        if(key=='2')
        {

            goto getConfigstart; // to get original pos conf with DB allready knowing POS serialnumber
        }

    getMerchantIDX:
    resp = getText(merchantIDX,3,0,"Enter merchantIDX");
      if(resp==1)
          goto sysctrl;// TBC   
      else{
         
          goto getConfigstart;
          
      }
        
        
    
        
    getConfigstart:   
        if(online)
        {
            CTOS_LCDTClearDisplay();
            PrintStringCenter(3,"Getting Config");
            if(fetchConfig())
            {
                 if(stoConfig()){
                     if(isFirstTimeConfig)
                          isFirstTimeConfig=FALSE;
                     
                     showError("Config Updated");
                 }
                 else
                     showError("Update Failure");
            }
            else
            {
                showError("Failed !");                
            }
            getConfig();
            goto sysctrl;
        }
        else
        {
            CTOS_LCDTClearDisplay();
            PrintStringCenter(3,"System Offline");
            PrintStringCenter(5,"Connect?");
            CTOS_LCDForeGndColor(0x0000ff);
            CTOS_LCDTPrintXY(9,8,"[X]");
            CTOS_LCDForeGndColor(0x21cd3a);
            CTOS_LCDTPrintXY(13,8,"[OK]");
            CTOS_LCDForeGndColor(0x000000);
    
            while(1)
            {
                CTOS_KBDGet(&key);
                if(key==d_KBD_CANCEL)
                        goto sysctrl;
                if(key==d_KBD_ENTER)
                        break;
            }
            CTOS_LCDTClearDisplay();
            if(gprsLan==TRUE)
                PrintStringCenter(3,"Opening GPRS ...");
            else
                PrintStringCenter(3,"Opening LAN ...");
            if(openGprs())
            {
                CTOS_LCDTClearDisplay();
                PrintStringCenter(3,"Getting Config");
                if(fetchConfig())
                {
                        if(stoConfig()){
                     if(isFirstTimeConfig)
                          isFirstTimeConfig=FALSE;
                     
                         showError("Config Updated");
                         }
                     else
                                showError("Update Failure");
                }
                else
                {
                        showError("Failed !");                
                }
                closeGprs();           
        }
        else
        {
             showError("Failed !");
        }
               showError("Disconnected");
         }
        getConfig();
        goto sysctrl;
    }
    if(key=='4')
    {
        if(getTranCountPend()>0)
        {
                CTOS_LCDTClearDisplay();
                CTOS_Beep();
                PrintStringCenter(2,"WARNING !");
                PrintStringCenter(4,"Pending");
                PrintStringCenter(5,"Transactions");
                PrintStringCenter(6,"Exist !!");
        
                CTOS_LCDTPrintXY(1,8,"                ");
                CTOS_LCDTPrintXY(1,8,"Cont.?");
                CTOS_LCDForeGndColor(0x0000ff);
                CTOS_LCDTPrintXY(9,8,"[X]");
                CTOS_LCDForeGndColor(0x21cd3a);
                CTOS_LCDTPrintXY(13,8,"[OK]");
                CTOS_LCDForeGndColor(0x000000);
                while(1)
                {
                        CTOS_KBDGet(&key);
                        if(key==d_KBD_CANCEL)
                                goto sysctrl;
                        if(key==d_KBD_ENTER)
                                break;
                }
        }
        CTOS_LCDTClearDisplay();
        CTOS_Beep();
        PrintStringCenter(2,"WARNING !");
        PrintStringCenter(4,"This will clear");
        PrintStringCenter(5,"all transactions");
        CTOS_LCDTPrintXY(1,8,"                ");
        CTOS_LCDTPrintXY(1,8,"Cont.?");
        CTOS_LCDForeGndColor(0x0000ff);
        CTOS_LCDTPrintXY(9,8,"[X]");
        CTOS_LCDForeGndColor(0x21cd3a);
        CTOS_LCDTPrintXY(13,8,"[OK]");
        CTOS_LCDForeGndColor(0x000000);
        while(1)
        {
            CTOS_KBDGet(&key);
            if(key==d_KBD_CANCEL)
                goto sysctrl;
            if(key==d_KBD_ENTER)
                break;
        }
        if(deleteTran()==TRUE)
        {
            showError("Cleared");
        }
        else
        {
            showError("Clear Failed !");
            goto sysctrl;
        }
        goto sysctrl;
    }
    if(key=='5')
    {
        exit(0);   
    }
    if(key=='6')
    {
         CTOS_LCDTClearDisplay();
        PrintStringCenter(3,"Powering Off");
        CTOS_PowerOff();
        exit(0);
    }
    if(key=='7')
    {
        memset(display,0,sizeof(display));
        CTOS_LCDTClearDisplay();
        PrintStringCenter(3,"Polling ....");
        if(pollSetup(display))
        {
            PrintStringCenter(5,display);
            if(stoMerchant(display))
            {
                PrintStringCenter(3,"Setup Updated");
                
            }
        }
        else
        {
            PrintStringCenter(5,"No Response");
        }
        CTOS_Delay(1500);
    }
    if(key=='8')
    {
        CTOS_LCDTClearDisplay();
        CTOS_Beep();
        PrintStringCenter(2,"WARNING !");
        PrintStringCenter(4,"This will clear");
        PrintStringCenter(5,"configuration");
        CTOS_LCDTPrintXY(1,8,"                ");
        CTOS_LCDTPrintXY(1,8,"Cont.?");
        CTOS_LCDForeGndColor(0x0000ff);
        CTOS_LCDTPrintXY(9,8,"[X]");
        CTOS_LCDForeGndColor(0x21cd3a);
        CTOS_LCDTPrintXY(13,8,"[OK]");
        CTOS_LCDForeGndColor(0x000000);
        while(1)
        {
            CTOS_KBDGet(&key);
            if(key==d_KBD_CANCEL)
                goto sysctrl;
            if(key==d_KBD_ENTER)
                break;
        }
        if(deleteConf()==TRUE)
        {
            showError("Config Cleared");
        }
        else
        {
            showError("Clear Failed !");
            goto sysctrl;
        }
        goto sysctrl;
    }
    goto sysctrl;
    
    enquire:
    
    CTOS_LCDTClearDisplay();
    PrintStringCenter(1,"Balance Enquiry");
    CTOS_LCDTPrintXY(1, 6, " Present  Card  ");
    CTOS_LCDForeGndColor(0x0000ff);
    CTOS_LCDTPrintXY(9, 8, "[X]");
    CTOS_LCDForeGndColor(0x000000);
    CTOS_LCDTPrintXY(13, 8, "Exit");
    
    retCard=StartTrans(0,cardNumber,cardID,lowb);
    if(retCard>-1)
    {
       CTOS_LCDTPrintXY(1,8,"                ");
       CTOS_LCDTPrintXY(1, 4,cardID);
        
        sprintf(cardBalance,"%7d",retCard);
        scrnFormat(cardBalance,balZ,7,FALSE,TRUE);
        CTOS_LCDTPrintXY(1, 6, "                ");
        strcpy(balDisp,"Balance:");
        strcat(balDisp,balZ);
        CTOS_LCDTPrintXY(1, 6,balDisp);
        if(sCrDr[0]=='C')
        {
                   CTOS_LCDTPrintXY(1,8,"                ");
                CTOS_LCDTPrintXY(1,8,"Print?");
                CTOS_LCDForeGndColor(0x0000ff);
                CTOS_LCDTPrintXY(9,8,"[X]");
                CTOS_LCDForeGndColor(0x21cd3a);
                CTOS_LCDTPrintXY(13,8,"[OK]");
                CTOS_LCDForeGndColor(0x000000);
                while(1)
                {
                        CTOS_KBDGet(&key);
                        if(key==d_KBD_CANCEL)
                                goto restart;
                        if(key==d_KBD_ENTER)
                                break;
                }
                CTOS_LCDTClearDisplay();
                PrintStringCenter(3,"Balance Enquiry");
                PrintStringCenter(5,"Please Wait ...");
                doLog(cardID,balZ);
                goto restart;
        }
        CTOS_Delay(2000);
        goto restart;
    }
    else
    {
        CTOS_LCDTPrintXY(1,8,"                ");
        CTOS_LCDTPrintXY(1, 6, "  Enquiry Fail  ");
        CTOS_Delay(2000);
        goto restart;
    }
 
    sale:
    
    
        
    redo:
    //
    memset(keyy,0,sizeof(keyy));
    amount=0;
    
redo_amt:
       cancel = FALSE;
	CTOS_LCDTClearDisplay();
       if(sCrDr[0]=='C')
       {
                PrintStringCenter(1,"Card Credit");
                strcpy(type,"10");
                strcpy(zAmount,"10");
                goto collected;
       }         
        else
        {
                PrintStringCenter(1,"Card Sale");
                CTOS_LCDForeGndColor(0x0000ff);
        CTOS_LCDTPrintXY(9,8,"[X]");
        CTOS_LCDForeGndColor(0x21cd3a);
        CTOS_LCDTPrintXY(13,8,"[OK]");
        CTOS_LCDForeGndColor(0x000000);
	strcpy(zAmount,"  ");

                        
                        
		disp:
				
			CTOS_LCDTPrintXY(1,4,"Vouchers:       ");		
			CTOS_LCDTPrintXY(12,4,zAmount);		
    	    while(1)
			{
				CTOS_KBDGet(&key);
				if(key==d_KBD_CANCEL)
				{
					cancel = TRUE;
					break;
				}
				if((key>='0')&&(key<='9')&(zAmount[0] == 0x20))
				{
                                    zAmount[2] = key;
					for(ii = 0; ii <2; ii++)
					{
						zAmount[ii] =zAmount[ii+1];
					}
                                    zAmount[2]=0x00;
					CTOS_LCDTPrintXY(1,4,"Vouchers: ");		
					CTOS_LCDTPrintXY(12,4,zAmount);		
				}
                                if(ASCII2Int(zAmount,2)>=10)
                                    break;
                                
				if(key==d_KBD_ENTER)
				{
                                   
					break;
				}
			}
			if(cancel)
			{
				showError("Cancelled !");
				goto restart;
			}
                        if(zAmount[0]==' ')
                            zAmount[0]='0';
                        zAmount[2]=0x00;
                        amount=ASCII2Int(zAmount,2);
                        strcpy(type,"00");
                        if(amount>10)
                            goto redo_amt;

        }
    
    
   
        
    
    
    collected:
    
    amount=StrToInt(zAmount);
    if(amount==0)
        goto redo;

    
 
    CTOS_LCDTPrintXY(1,8,"                ");
    CTOS_LCDForeGndColor(0x0000ff);
    CTOS_LCDTPrintXY(9, 8, "[X]");
    CTOS_LCDForeGndColor(0x000000);
    CTOS_LCDTPrintXY(13, 8, "Exit");
    CTOS_LCDTPrintXY(1, 6, " Present  Card  ");
    sprintf(cardAmount,"%08d",amount);
    
    retCard=StartTrans(amount,cardNumber,cardID,lowb);
    
    if(retCard>-1)
    {
 
                    
        sprintf(cardBalance,"%08d",retCard);
 //BOOL stoTran("",STR tRef[],STR tGross[],STR tDisc[],STR tTip[],STR tNett[],STR tBal[],STR tSer0[],STR tSer1[],STR tId0[],STR tId1[],STR tPurse[],STR tLpoints[],STR tLbal[],BOOL sent)
        stoTran(type,"00000000",cardAmount,"00000000","00000000",cardAmount,cardBalance,cardNumber,"FFFFFFFF",cardID,"                ","T1","FFFFFF","FFFFFFFF",FALSE);
        if((cardID[0]=='0')&&(cardID[1]=='0')&&(cardID[2]=='0')&&(cardID[3]=='0'))
            addToTotalP(amount);
        else
            addToTotalC(amount);
	
        CTOS_LCDTClearDisplay();
        PrintStringCenter(2,"Successful!");       
        sprintf(cardBalance,"%7d",retCard);
        scrnFormat(cardBalance,balX,7,FALSE,TRUE);
        strcpy(balDisp,"Balance:");
        strcat(balDisp,balX);
        PrintStringCenter(5,balDisp);
        if(sRcptA)
        { 
            sprintf(dispAmtX,"% 7d",amount);
            doPrint(cardName,cardID,sMerchantName,sDisp,dispAmtX,balX,TRUE);
        }
    }
    else
    {
        CTOS_LCDTClearDisplay();
        PrintStringCenter(2,"Transaction FAIL");
        if(strlen(lowb)>0)
        {
            PrintStringCenter(4,"Low Funds !");
            strcpy(balDisp,"Balance:");
            strcat(balDisp,lowb);
            PrintStringCenter(6,balDisp); 
        }
    }
    CTOS_LCDTPrintXY(1,8,"                ");
    CTOS_LCDTPrintXY(1,8,"Continue");
    CTOS_LCDForeGndColor(0x21cd3a);
    CTOS_LCDTPrintXY(13,8,"[OK]");
    CTOS_LCDForeGndColor(0x000000);
    while(1)
    {
        CTOS_KBDGet(&key);
        if(key==d_KBD_ENTER)
            break;
    }
     if((doUpload==TRUE)&&(online==TRUE))
    {
        if(getTranCountPend()>0)
            uploadTran();
    }
    goto restart;
}
//
//      LCD Routines
//


int getText(STR output[],int maxLength,int type,STR legend[])
{
    STR curSTRCapture[16];
    STR *keyboardLayoutAlpha[]={" ", "QZ", "ABC","DEF", "GHI", "JKL", "MNO", "PRS", "TUV","WXY", ":;/\\|?,.<>", ".!@#$%^&*()"};
    STR *keyboardLayoutNumericAlpha[]={"0 ", "1QZ", "2ABC","3DEF", "4GHI", "5JKL", "6MNO", "7PRS", "8TUV","9WXY", ":;/\\|?,.<>", ".!@#$%^&*()"};
    STR *keyboardLayoutAlphaNumeric[]={" 0", "QZ1", "ABC2","DEF3", "GHI4", "JKL5", "MNO6", "PRS7", "TUV8","WXY9", ":;/\\|?,.<>", ".!@#$%^&*()"};
    STR *keyboardLayoutNumeric[]={"0", "1", "2", "3", "4", "5","6", "7", "8", "9", "", ""};
    CTOS_LCDTPrintXY(1,3,"                ");
    CTOS_LCDTPrintXY(1,4,"                ");
    CTOS_LCDTPrintXY(1,5,"                ");
    CTOS_LCDTPrintXY(1,6,"                ");
    CTOS_LCDTPrintXY(1,7,"                ");
    CTOS_LCDTPrintXY(1,7,"                ");
    CTOS_LCDTPrintXY(1,8,"                ");
    CTOS_LCDForeGndColor(0x00ffff);
    CTOS_LCDTPrintXY(1,8,"[CLR]");
    CTOS_LCDForeGndColor(0x21cd3a);
    CTOS_LCDTPrintXY(13,8,"[OK]");
    CTOS_LCDForeGndColor(0x000000);
    PrintStringCenter(3,legend);
    output[0]=0x00;
    if(type==0)
    {
        CTOS_UIKeypad(1, 5, keyboardLayoutNumeric, 40, 80, d_TRUE,d_FALSE, 0, 0, output, maxLength+1);
    }
    if(type==1)
    {
        CTOS_UIKeypad(1, 5, keyboardLayoutAlpha, 40, 80, d_TRUE,d_FALSE, 0, 0, output, maxLength+1);
    }
    if(type==2)
    {
        CTOS_UIKeypad(1, 5, keyboardLayoutNumericAlpha, 40, 80, d_TRUE,d_FALSE, 0, 0, output, maxLength+1);
    }
    if(type==3)
    {
        CTOS_UIKeypad(1, 5, keyboardLayoutAlphaNumeric, 40, 80, d_TRUE,d_FALSE, 0, 0, output, maxLength+1);
    }
    
    CTOS_PrinterPutString("\n\n\n\n\n\n");
    CTOS_PrinterPutString("*****************");   
    
    memset(curSTRCapture,0,sizeof(curSTRCapture) );
    sprintf(curSTRCapture,"Keyboard Output:  %s",output);
    CTOS_PrinterPutString(curSTRCapture);
    CTOS_PrinterPutString("*****************");  
    CTOS_PrinterPutString(output);  
    CTOS_PrinterPutString("\n\n\n\n\n\n");
    if(output[0]==0x00){
        CTOS_PrinterPutString("Keyb Returning 1 \n\n\n\n\n\n");    
        return 1;
    }
     CTOS_PrinterPutString("Keyb Returning 0 \n\n\n\n\n\n");        
    return 0;
    
}
void setLCD()
{
        CTOS_LCDFontSelectMode(d_FONT_FNT_MODE);
        CTOS_LCDBackGndColor(0xfdf6b3);
        CTOS_LCDSelectMode(d_LCD_TEXT_MODE);//////
        CTOS_LCDTClearDisplay();
        CTOS_LCDForeGndColor(0x000000);
        CTOS_LCDTSelectFontSize(d_LCD_FONT_8x8);   
}
void PrintStringCenter(BYTE y,STR *sBuf)
{
	CTOS_LCDTPrintXY(1,y,"                ");
	CTOS_LCDTPrintXY((16-strlen(sBuf))/2+1,y,sBuf);
}
void PrintStringCenter2lINES(BYTE y,BYTE y2,STR *sBuf,STR *sBuf2)
{
	CTOS_LCDTPrintXY((16-strlen(sBuf))/2+1,y,sBuf);
        CTOS_LCDTPrintXY((16-strlen(sBuf2))/2+1,2,sBuf2);
}
//
//      RTC Routines
//
void getTimeDate()
{
        CTOS_RTC stRTC;
        CTOS_RTCGet(&stRTC);
        sprintf(dispTime,"%02d:%02d",(int)stRTC.bHour,(int)stRTC.bMinute);
        sprintf(dispDate,"%02d/%02d/%02d", (int)stRTC. bDay, (int)stRTC.bMonth, (int)stRTC.bYear);
        sprintf(tranTime,"%02d%02d%02d",(int)stRTC.bHour,(int)stRTC.bMinute,(int)stRTC.bSecond);
        sprintf(tranDate,"%02d%02d%02d", (int)stRTC. bDay, (int)stRTC.bMonth, (int)stRTC.bYear);
        sprintf(baDateTime,"%04d%02d%02d%02d%02d%02d",(int)stRTC.bYear,(int)stRTC.bMonth,(int)stRTC. bDay,(int)stRTC.bHour,(int)stRTC.bMinute,(int)stRTC.bSecond);
        sprintf(expiryDate,"%02d%02d",(int)stRTC.bYear,(int)stRTC.bMonth);
        strcpy(dispTD,dispTime);
        strcat(dispTD,"   ");
        strcat(dispTD,dispDate);
}
int ASCII2Int(BYTE *baSBuf,int iL)
{
	int i,iN,k;
	iN=0;
	k=1;
	for(i=iL-1;i>=0;i--)
	{
		iN=(int)((baSBuf[i]-'0')*k)+iN;
		k*=10;
	}
	return iN;
}

void setTimeDate()
{
	STR xData[20];
	STR tempT[7];
	STR tempD[7];
	STR temp[8];
        CTOS_RTC stRTC;
        CTOS_RTCGet(&stRTC);
	sprintf(tempT,"%02d%02d",(int)stRTC.bHour,(int)stRTC.bMinute);
	sprintf(tempD,"%02d%02d%02d", (int)stRTC. bDay, (int)stRTC.bMonth, (int)stRTC.bYear);

	if(doCollect("SET TIME",7,5,4,xData,tempT,FALSE))
	{
		temp[0] = xData[0];
		temp[1] = xData[1];
		temp[2] = 0x00;
		stRTC.bHour=StrToInt(temp);
		temp[0] = xData[2];
		temp[1] = xData[3];
		temp[2] = 0x00;
		stRTC.bMinute=StrToInt(temp);
		stRTC.bSecond=0;
	}
	else
	{
		showError("Canceled");
	}

	if(doCollect("SET DATE",6,5,6,xData,tempD,FALSE))
	{
		temp[0] = xData[0];
		temp[1] = xData[1];
		temp[2] = 0x00;
		stRTC.bDay=StrToInt(temp);
		temp[0] = xData[2];
		temp[1] = xData[3];
		temp[2] = 0x00;
		stRTC.bMonth=StrToInt(temp);
		temp[0] = xData[4];
		temp[1] = xData[5];
		temp[2] = 0x00;
		stRTC.bYear=StrToInt(temp);
		
		if(CTOS_RTCSet(&stRTC)==d_OK)
		{
			showError("Time/Date Set");
		}
		else
		{
			showError("Time/Date Fail!");
		}
	}
	else
	{
		showError("Canceled");
	}
}
//
//      Keypad Routines
//
BOOL doCollect(STR heading[],int positionX,int positionY,int maxChar,STR output[],STR initial[],BOOL pwd)
{
    int cnt;
    STR pin[5];
    STR initialX[20];
	cnt = 0;
	CTOS_LCDTClearDisplay();;
	PrintStringCenter(1,heading);
	strcpy(initialX,"[");
	strcat(initialX,initial);
	strcat(initialX,"]");
        CTOS_LCDTPrintXY(positionX-1,3,initialX);
	while(1)
	{
		CTOS_KBDGet(&key);
		if(key==d_KBD_ENTER)
		{
			if(cnt>0)
			{
				return TRUE;
			}
			if(cnt == 0)
			{
				for(ii = 0 ;ii< maxChar; ii++)
				{
					output[ii] = initial[ii];
                                        pin[ii]='#';
				}
				output[maxChar] = 0x00;
                                pin[maxChar]=0x00;
				return TRUE;
			}
		}
		if(key==d_KBD_CANCEL)
			return FALSE;
			
		if(((key>='0')&&(key<='9')))
		{
			output[cnt] = key;
                        pin[cnt]='#';
			cnt ++;
			output[cnt] = 0x00;
                        pin[cnt]=0x00;
                        if(pwd==FALSE)
                            CTOS_LCDTPrintXY(positionX,positionY,output);
                        else
                            CTOS_LCDTPrintXY(positionX,positionY,pin);
                            
	  		if(cnt==maxChar)
	  			return TRUE;
		}
	}
}
//
//      Comms Routines
//
void packSub(int offset, int length, STR in[], STR out[])
{
    int fCnt=0;
    for(i=0;i<length;i++)
    {
        out[i]=in[i+offset];
        if(out[i]=='F')
            fCnt++;
    }
    out[length]=0x00;
    if(fCnt==length)
        out[0]=0x00;
}
void packTran(STR transaction[])
{
    STR packTemp[130];
    getSerial();
    memset(msgXml,0,sizeof(msgXml));
    strcpy(msgXml,"<PF>");
    strcat(msgXml,"<TERM>");
    strcat(msgXml,termSerial);
    strcat(msgXml,"</TERM>");
    strcat(msgXml,"<FLAG>T062NNN</FLAG>");
    packSub(62,2,transaction,packTemp);
    strcat(msgXml,"<PURSE>");
    strcat(msgXml,packTemp);
    strcat(msgXml,"</PURSE>");
   
    packSub(64,8,transaction,packTemp);
    strcat(msgXml,"<SER0>");
    strcat(msgXml,packTemp);
    strcat(msgXml,"</SER0>");
    packSub(72,8,transaction,packTemp);
       strcat(msgXml,"<SER1>");
        strcat(msgXml,packTemp);
        strcat(msgXml,"</SER1>");
      packSub(80,16,transaction,packTemp);
        strcat(msgXml,"<ID0>");
        strcat(msgXml,packTemp);
        strcat(msgXml,"</ID0>");
       
    packSub(96,16,transaction,packTemp);
    
        strcat(msgXml,"<ID1>");
        strcat(msgXml,packTemp);
        strcat(msgXml,"</ID1>");
       
    packSub(112,14,transaction,packTemp);
    
        strcat(msgXml,"<LP>");
        strcat(msgXml,packTemp);
        strcat(msgXml,"</LP>");
    
 //       testData(msgXml);
 
    strcat(msgXml,"<DB>|");
    strcat(msgXml,"10.0.0.50");//"197.242.146.179";
    strcat(msgXml,"|");
    strcat(msgXml,"payfare");
    strcat(msgXml,"|");
    strcat(msgXml,sTableHdr);
    strcat(msgXml,"tran");
    strcat(msgXml,"|payfare|p4yf4r3|");
    strcat(msgXml,"</DB>");
    strcat(msgXml,"<DATA>");
    packSub(0,62,transaction,packTemp);
    strcat(msgXml,packTemp);
    strcat(msgXml,"</DATA>");    
    strcat(msgXml,"</PF>");
    strcat(msgXml,"\r");
    strcat(msgXml,"\n");
    
//    testData(msgXml);
}

void packSetup()
{
    PrintorDisplayMSG("*****************\n");
    PrintorDisplayMSG("Pack Setup start\n");
    
    getSerial();
    
    PrintorDisplayMSG("POS S/N:");
     PrintorDisplayMSG(termSerial);
     PrintorDisplayMSG("\n\n");
     
     PrintorDisplayMSG("MERC IDX:");
     PrintorDisplayMSG(merchantIDX);
     PrintorDisplayMSG("\n\n");
    
    memset(termSerialFinal,0,sizeof(termSerialFinal));
    if(!isFirstTimeConfig)
        strcpy(termSerialFinal,termSerial);
    else{
       strcpy(termSerialFinal,merchantIDX);
       strncpy(termSerialFinal+3,termSerial,10);
    }
    
    PrintorDisplayMSG("\n\n Pack Setup Final S/N");
    PrintorDisplayMSG("*****************");   
    PrintorDisplayMSG(termSerialFinal);
    PrintorDisplayMSG("*****************");   
    PrintorDisplayMSG("\n\n");
    //Possibly area to put request for config id and put serial number update for db
   
    memset(msgXml,0,sizeof(msgXml));
    strcpy(msgXml,"<PF><TERM>");
    strcat(msgXml,termSerialFinal);
    strcat(msgXml,"</TERM>");
    strcat(msgXml,"<FLAG>C000NNN</FLAG>");
    strcat(msgXml,"<DB>");
    strcat(msgXml,configDb); //This array is setup in the beginning of the app with: strcpy(configDb,"|10.0.0.50|bubbly2017|termsetup|payfare|p4yf4r3|");" 
    strcat(msgXml,"</DB>");
    strcat(msgXml,"</PF>");
    strcat(msgXml,"\r");
    strcat(msgXml,"\n");
    
    CTOS_PrinterPutString("\n\n");
    CTOS_PrinterPutString(msgXml);
    CTOS_PrinterPutString("\n\n\n Pack Setup Message End\n");
    PrintorDisplayMSG("*****************\n\n\n\n\n");
}


void PrintorDisplayMSG(UCHAR* baMSGBuf){
    
    if(CTOS_PrinterStatus()==d_OK)
    {
        CTOS_PrinterPutString(baMSGBuf);
    }else
    {
        CTOS_LCDTClearDisplay();;
	PrintStringCenter(1,baMSGBuf);
    }
    
    
}
void packPoll()
{
    getSerial();
    memset(msgXml,0,sizeof(msgXml));
    strcpy(msgXml,"<PF><TERM>");
    strcat(msgXml,termSerial);
    strcat(msgXml,"</TERM>");
    strcat(msgXml,"<FLAG>P000NNN</FLAG>");
    strcat(msgXml,"<DB>|");
    strcat(msgXml,"10.0.0.50");
    strcat(msgXml,"|");
    strcat(msgXml,"payfare");
    strcat(msgXml,"|");
    strcat(msgXml,sTableHdr);
    strcat(msgXml,"poslog");
    strcat(msgXml,"|payfare|p4yf4r3|");
    strcat(msgXml,"</DB>");
    strcat(msgXml,"</PF>");
    strcat(msgXml,"\r");
    strcat(msgXml,"\n");
}

BOOL getTag(STR in[],STR tag[], STR out[])
{
    STR tag1[20];
    STR tag2[20];
    int start=-1;
    int end=-1;
    BOOL found=FALSE;
    strcpy(tag1,"<");
    strcat(tag1,tag);
    strcat(tag1,">");
    strcpy(tag2,"</");
    strcat(tag2,tag);
    strcat(tag2,">");
    
    for(i=0;i<strlen(in)-strlen(tag1);i++)
    {
        if(tag1==in[i])
        {
            found=TRUE;
            for(j=0;j<strlen(tag1);j++)
            {
                if(tag1[j]!=in[i+j])
                    found=FALSE;
            }
            if(found)
                break;
        }
        if(found)
                break;
    }
    start=i+strlen(tag1);
    
    for(i=start;i<strlen(in)-start-strlen(tag2);i++)
    {
        if(tag1==in[i])
        {
            found=TRUE;
            for(j=0;j<strlen(tag2);j++)
            {
                if(tag2[j]!=in[i+j])
                    found=FALSE;
            }
            if(found)
                break;
        }
        if(found)
                break;
    }
    end=i;
    
    for(i=start;i<end;i++)
    {
        out[i-start]=in[i];        
    }
    out[end-start]=0x00;
}

USHORT pollStatusResult()
{
    USHORT usret;
    DWORD status;
    status = 0;
    while(CTOS_TCP_GPRSStatus(&status) == d_TCP_IO_PROCESSING)
    {
        CTOS_Delay(10);
    }
    usret = CTOS_TCP_GPRSStatus(&status);
    return usret;
}
BOOL openGprs()
{
    online=FALSE;
    if(gprsLan==TRUE)
    {
            CTOS_TCP_GPRSOpen(baLocalIP ,APN ,"" ,"");
            
        usrtn = pollStatusResult();
        if (usrtn != d_OK)
            return FALSE;
        usrtn = CTOS_TCP_GPRSGetIP(baLocalIP);
        if(usrtn != d_OK)
            return FALSE;
        
        online=TRUE;
        return TRUE;
    }
    else
    {
        usrtn=CTOS_EthernetOpen();
        if (usrtn != d_OK)
        {
            CTOS_EthernetClose();
            return FALSE;
        }
        online=TRUE;
        return TRUE;
    }
}
BOOL closeGprs()
{
    if(gprsLan==TRUE)
    {
        CTOS_TCP_GPRSClose();
        usrtn = pollStatusResult();
        if (usrtn != d_OK)
        {
            CTOS_TCP_GPRSClose();
            usrtn = pollStatusResult();
            if (usrtn != d_OK)
                return FALSE;
            online=FALSE;
            return TRUE;
        }
        online=FALSE;
        return TRUE;
    }
    else
    {
        CTOS_EthernetClose();
        online=FALSE;
        return TRUE;
    }
}
BOOL connectGprs()
{
     STR test[10];
    if(gprsLan==TRUE)
    {
        usrtn = CTOS_TCP_GPRSConnectEx(&iSocket ,baRemotelIP ,usPort);
        usrtn = pollStatusResult();
        if (usrtn != d_OK)
        {
            return FALSE;
        }
        return TRUE;
    }
    else
    {
        usrtn = CTOS_EthernetConnect();
        if (usrtn != d_OK)
        {
            sprintf(test,"%02x",usrtn);
            CTOS_EthernetDisconnect();
            return FALSE;
        }
        return TRUE;
    }
}
BOOL disconnectGprs()
{
    if(gprsLan==TRUE)
    {
        CTOS_TCP_GPRSDisconnect(iSocket);
        pollStatusResult();
    }
    else
    {
        CTOS_EthernetDisconnect();
    }
}
BOOL txrxGprs()
{
     DWORD dwStatus;
    USHORT usr_Len = 0, usR_Len = 0;
    BOOL response=FALSE;
    USHORT totalLength=0;
    USHORT timeOut=0;
    int start,end,cntr,cntr1;
    memset(rxData,0,sizeof(rxData));
    memset(rxDataX,0,sizeof(rxDataX));
    if(gprsLan==TRUE)
    {
        CTOS_TCP_GPRSTx(iSocket ,msgXml ,strlen(msgXml));
        usrtn = pollStatusResult();
        if (usrtn != d_OK)
            return FALSE;
        
        while(!response)
        {
            usRecvLen=sizeof(rxDataX);
            CTOS_TCP_GPRSRx(iSocket ,rxDataX ,&usRecvLen);
            usrtn = pollStatusResult();
            if(usrtn!=d_OK)
                return FALSE;
            totalLength += usRecvLen;
            rxDataX[usRecvLen]=0x00;
            strcat(rxData,rxDataX);
            CTOS_Delay(100);
            timeOut++;
            if(timeOut>50)
                return FALSE;
            if(totalLength>10)
            {
                for(i=0;i<totalLength-1;i++)
                {
                    if((rxData[i]=='/')&&(rxData[i+1]=='R')&&(rxData[i+2]=='E')&&(rxData[i+3]=='S')&&(rxData[i+4]=='P')&&(rxData[i+5]=='>'))
                    {
                        usrtn=totalLength;
                        response=TRUE;
                    }
                }
            }
        }
    }
    else
    {
        usrtn = CTOS_EthernetStatus(&dwStatus);
        if(usrtn != d_OK)
            return FALSE;
        CTOS_EthernetTxData(msgXml ,strlen(msgXml));
        usRecvLen=sizeof(rxData);
        CTOS_Delay(500);
        memset(rxData,0x00,sizeof(rxData));
        usr_Len = sizeof(rxData);
        while(1)
        {
            usrtn = CTOS_EthernetRxReady(&usr_Len);
            if (usrtn != d_OK)
            {
                CTOS_EthernetFlushRxData();
                return FALSE;
            }
            else
            {
                CTOS_EthernetRxData(&rxData[usR_Len],&usr_Len);
                usR_Len += usr_Len;
                break;
            }
        }
        usrtn=usR_Len;
        CTOS_EthernetFlushRxData();
    }
    
    if(usrtn > 0)
    {
        memset(rxDataX,0,sizeof(rxDataX));
        start = 0;
        end = 0;
        for(cntr = 0; cntr < usrtn-1; cntr ++)
        {
            if((rxData[cntr] == 'P')&&(rxData[cntr+1] == '>'))
            {
                start = cntr+2;
                break;
            }
        }
        for(cntr = start; cntr < usrtn-3; cntr ++)
        {
            if((rxData[cntr] == '<')&&(rxData[cntr+1] == '/')&&(rxData[cntr+2] == 'R'))
            {
                end = cntr;
                break;
            }
        }
        cntr1 = 0;
        if((start>0)&&(end>0)&&(end>start))
        {
            for(cntr = start; cntr < end; cntr ++)
            {
                rxDataX[cntr1] = rxData[cntr];
                cntr1++;
            }
            rxDataX[cntr1] = 0x00;
            return TRUE;
        }
        else
        {
            return FALSE;	
        }
    }
    else
    {
        return FALSE;
    }
    return FALSE;
}
BOOL pollTran(STR tran[])
{
    packTran(tran);
    if(txrxGprs())
    {
        if((rxDataX[0]=='O')&&(rxDataX[1]=='K'))
            return TRUE;
        //testData(rxDataX);
        return FALSE;
    }
    else
    {
        
        
        return FALSE;
    }
}
BOOL pollSetup(BYTE display[])
{
    int cntr,cntr1,start,end,datalen;
    memset(display,0,sizeof(display));
    packSetup();
    if(connectGprs())
    {
        if(txrxGprs())
        {
            disconnectGprs();
            datalen=strlen(rxDataX);
            memset(display,0,sizeof(display));
            start = 0;
            end = 0;
            for(cntr = 0; cntr < datalen-1; cntr ++)
            {
                if((rxDataX[cntr] == 'P')&&(rxDataX[cntr+1] == '>'))
		{
                    start = cntr+2;
                    break;
		}
            }
            for(cntr = start; cntr < datalen-3; cntr ++)
            {
                if((rxDataX[cntr] == '<')&&(rxDataX[cntr+1] == '/')&&(rxDataX[cntr+2] == 'D')&&(rxDataX[cntr+3] == 'I'))
		{
                    end = cntr;
                    break;
		}
            }
            cntr1 = 0;
            if((start>0)&&(end>0)&&(end>start))
            {
                for(cntr = start; cntr < end; cntr ++)
		{
                    display[cntr1] = rxDataX[cntr];
                    cntr1++;
		}
                display[cntr1] = 0x00;
                return TRUE;
            }
            return FALSE;
        }
        else
        {
            disconnectGprs();
            return FALSE;
        }
     }
     else
     {
        return FALSE;
     }
}
BOOL pollNetwork()
{
     packPoll();
    if(connectGprs())
    {
        if(txrxGprs())
        {
            disconnectGprs();
            return TRUE;
        }
        else
        {
            disconnectGprs();
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}
BOOL fetchConfig()
{
    //this gets config from Database
    STR dataReceived[240];
    STR dispTemp[40];
    int cntr,cntr1,start,end,datalen,kk,kl,km;
    int xy=0; 
    int ovXfl = 0;
    /*STR sDispoverflow[20];
      BOOL bDisplayIsOverFlowing;
     * */
 
            
    packSetup();// Packet setup prepares the TCP message to be sent specifically for Config
    if(connectGprs()) //Connect GPRS does both GPRS and LAN connection
    {
        if(txrxGprs())
        {
//            testData(rxDataX);
            disconnectGprs();
            datalen=strlen(rxDataX);
            memset(dispTemp,0,sizeof(dispTemp));
            memset(sTableHdr,0,sizeof(sTableHdr));
            memset(sMerchantName,0,sizeof(sMerchantName));
            memset(dataReceived,0,sizeof(dataReceived));
             start = 0;
            end = 0;
            for(cntr = 0; cntr < datalen-1; cntr ++)
            {
                if((rxDataX[cntr] == 'P')&&(rxDataX[cntr+1] == '>'))
		{
                    start = cntr+2;
                    break;
		}
            }
            for(cntr = start; cntr < datalen-3; cntr ++)
            {
                if((rxDataX[cntr] == '<')&&(rxDataX[cntr+1] == '/')&&(rxDataX[cntr+2] == 'D')&&(rxDataX[cntr+3] == 'I'))
		{
                    end = cntr;
                    break;
		}
            }
            
            cntr1 = 0;
            if((start>0)&&(end>0)&&(end>start))
            {
                for(cntr = start; cntr < end; cntr ++)
		{
                    dispTemp[cntr1] = rxDataX[cntr];
                    cntr1++;
		}
                dispTemp[cntr1] = 0x00;
                memset(sDisp,0,sizeof(sDisp));
                kk=strlen(dispTemp);//
                if(kk<15)
                {
                    kl=(16-kk)/2;
                    km=16-kk-kl;
                    for(i=0;i<kl;i++)
                    {
                        sDisp[i]=' ';
                    }
                    for(i=0;i<kk;i++)
                    {
                        sDisp[i+kl]=dispTemp[i];
                    }
                    for(i=0;i<km;i++)
                    {
                        sDisp[kk+kl+i]=' ';
                    }
                    sDisp[16]=0x00;
                }
                if(kk==15)
                {
                        strcpy(sDisp,dispTemp);
                        strcpy(sDisp," ");
                }
                if(kk==16)
                {
                        strcpy(sDisp,dispTemp);
                }
                /*STR sDispoverflow[20];
                        BOOL bDisplayIsOverFlowing;
                 * */
                if(kk>16)
                {
                    memset(sDispoverflow,0,sizeof(sDispoverflow));
                    strncpy(sDisp,dispTemp,16);
                        //sDisp[16]=0x00;
                    ovXfl=0;
                    for( xy = 16; xy < 32; xy ++)
                        {
                          sDispoverflow[ovXfl] = dispTemp[xy];
                          ovXfl++;     
                        }
                                             
                    bDisplayIsOverFlowing=TRUE;
                }
                
 //                testData(sDisp);
            }
            else
            {
                return FALSE;
            }
            start = 0;
            end = 0;
            for(cntr = 0; cntr < datalen-1; cntr ++)
            {
                if((rxDataX[cntr] == 'E')&&(rxDataX[cntr+1] == '>'))
		{
                    start = cntr+2;
                    break;
		}
            }
            for(cntr = start; cntr < datalen-3; cntr ++)
            {
                if((rxDataX[cntr] == '<')&&(rxDataX[cntr+1] == '/')&&(rxDataX[cntr+2] == 'T')&&(rxDataX[cntr+3] == 'A'))
		{
                    end = cntr;
                    break;
		}
            }
            cntr1 = 0;
            if((start>0)&&(end>0)&&(end>start))
            {
                for(cntr = start; cntr < end; cntr ++)
		{
                    sTableHdr[cntr1] = rxDataX[cntr];
                    cntr1++;
		}
                sTableHdr[cntr1] = 0x00;
 //                testData(sTableHdr);
            }
            else
            {
                sTableHdr[0]=0x00;
            }
            start = 0;
            end = 0;
            for(cntr = 0; cntr < datalen-1; cntr ++)
            {
                if((rxDataX[cntr] == 'H')&&(rxDataX[cntr+1] == '>'))
		{
                    start = cntr+2;
                    break;
		}
            }
            for(cntr = start; cntr < datalen-3; cntr ++)
            {
                if((rxDataX[cntr] == '<')&&(rxDataX[cntr+1] == '/')&&(rxDataX[cntr+2] == 'M')&&(rxDataX[cntr+3] == 'E'))
		{
                    end = cntr;
                    break;
		}
            }
            cntr1 = 0;
            if((start>0)&&(end>0)&&(end>start))
            {
                for(cntr = start; cntr < end; cntr ++)
		{
                    sMerchantName[cntr1] = rxDataX[cntr];
                    cntr1++;
		}
                sMerchantName[cntr1] = 0x00;
//                 testData(sMerchantName);
            }
            else
            {
                return FALSE;
            }
            start = 0;
            end = 0;
            for(cntr = 0; cntr < datalen-1; cntr ++)
            {
                if((rxDataX[cntr] == 'A')&&(rxDataX[cntr+1] == '>'))
		{
                    start = cntr+2;
                    break;
		}
            }
            for(cntr = start; cntr < datalen-3; cntr ++)
            {
                if((rxDataX[cntr] == '<')&&(rxDataX[cntr+1] == '/')&&(rxDataX[cntr+2] == 'D')&&(rxDataX[cntr+3] == 'A'))
		{
                    end = cntr;
                    break;
		}
            }
            cntr1 = 0;
            if((start>0)&&(end>0)&&(end>start))
            {
                for(cntr = start; cntr < end; cntr ++)
		{
                    dataReceived[cntr1] = rxDataX[cntr];
                    cntr1++;
		}
                dataReceived[cntr1] = 0x00;
 //                testData(dataReceived);
            }
            else
            {
                return FALSE;
            }
            for(i=0;i<4;i++)
            {
                sCountry[i]=dataReceived[i];
                sSystem[i]=dataReceived[i+4];
                sClient[i]=dataReceived[i+8];
                sKey[i]=dataReceived[i+12];
            }
            if(dataReceived[16]=='Y')
                sMerchCard=TRUE;
            else
                sMerchCard=FALSE;
            sCardType[0]=dataReceived[17];
            sCardType[1]=0x00;
            sTermType[0]=dataReceived[18];
            sTermType[1]=0x00;
            sUpload[0]=dataReceived[19];
            sUpload[1]=0x00;
            sCrDr[0]=dataReceived[20];
            sCrDr[1]=0x00;
            if(dataReceived[21]=='Y')
                sRcptA=TRUE;
            else
                sRcptA=FALSE;
            if(dataReceived[22]=='Y')
                sRcptB=TRUE;
            else
                sEnable=FALSE;
            if(dataReceived[23]=='E')
                sEnable=TRUE;
            else
            sLoyalty[0]=dataReceived[24];
            sLoyalty[1]=dataReceived[25];
            sLoyalty[2]=0x00;

            return TRUE;
        }
        else
        {
            disconnectGprs();
            return FALSE;
        }
     }
     else
     {
        return FALSE;
     }
}
BOOL uploadTran()
{
    int count,cntt;
    STR tranx[130];
    STR cntDisp[5];
    STR file[10];
    
    count=0;
    cntt=1;
    strcpy(file,"trans");
    CTOS_LCDTClearDisplay();;
    CTOS_LCDTPrintXY(1,5,"Connecting .....");
    if(connectGprs())
    {
        CTOS_LCDTPrintXY(1,5,"   Connected    ");
        while((getTran(count,tranx)==TRUE)&&(strlen(tranx)==128))
        {
            
            
         //   testData(tranx);    ////
            
            
            if((tranx[127]=='#')&&(tranx[126]=='F'))
            {
                sprintf(cntDisp,"%ld",cntt);
                CTOS_LCDTPrintXY(1,5," Uploading      ");
                CTOS_LCDTPrintXY(13,5,cntDisp);
                if(pollTran(tranx))
                {
                    tranx[126]='0';
                    if(stomem(file,tranx,count,TRUE)==TRUE)
                    {
                        cntt++;
                    }
                    else
                    {
                        disconnectGprs();
                        return FALSE;
                    }
                }
                else
                {
                    disconnectGprs();
                    return FALSE;
                }
            }
            count=count+128;
            if(tranx[127]!='#')
                break;
        }
        CTOS_LCDTPrintXY(1,5,"Disconnecting ..");
        disconnectGprs();   
        return TRUE;
    }
    else
    {
        showError("Connect Fail !");
        return FALSE;
    }
}
//
//      File Storage Routines
//
BOOL recreate(BYTE *fileName)
{
    ULONG ulFileSize=0, ulHandle, ret;
    ret=CTOS_FileGetSize(fileName,&ulFileSize);
    if(ret==d_OK)
    {
        if(CTOS_FileDelete(fileName)!=d_OK)
            return FALSE;
    }
    CTOS_Delay(50);
    ret=CTOS_FileOpen(fileName , d_STORAGE_FLASH , &ulHandle);
    if(ret!=d_OK)
        return FALSE;
    CTOS_FileClose(ulHandle);
    return TRUE;
}
BOOL balsto(BYTE *fileName,BYTE *data)
{
     ULONG ulFileSize = 0, ulHandle;
    if(CTOS_FileGetSize(fileName,&ulFileSize)== d_FS_FILE_NOT_FOUND)
    {
        CTOS_Delay(50);
        if(CTOS_FileOpen(fileName , d_STORAGE_FLASH , &ulHandle)!=d_OK)
        {
            return FALSE;
        }
          
        CTOS_FileClose(ulHandle);
    }
    CTOS_Delay(50);
    if(CTOS_FileReopen(fileName ,d_STORAGE_FLASH ,&ulHandle)!=d_OK)
        {
           return FALSE;
        }
          
    CTOS_FileSeek(ulHandle ,0 ,d_SEEK_FROM_BEGINNING);
    if(CTOS_FileWrite ( ulHandle, data, strlen(data))==d_OK)
    {
        CTOS_FileClose ( ulHandle );
        return TRUE;
    }
    CTOS_FileClose ( ulHandle );
    return FALSE;
}
BOOL stomem(BYTE *fileName,BYTE *data,int position,BOOL begin)
{
    ULONG ulFileSize = 0, ulHandle;
    if(CTOS_FileGetSize(fileName,&ulFileSize)== d_FS_FILE_NOT_FOUND)
    {
        CTOS_Delay(50);
        if(CTOS_FileOpen(fileName , d_STORAGE_FLASH , &ulHandle)!=d_OK)
        {
            return FALSE;
        }
          
        CTOS_FileClose(ulHandle);
    }
    CTOS_Delay(50);
    if(CTOS_FileReopen(fileName ,d_STORAGE_FLASH ,&ulHandle)!=d_OK)
        {
           return FALSE;
        }
    CTOS_Delay(50);      
    if(begin==TRUE)
    {
        CTOS_FileSeek(ulHandle ,position ,d_SEEK_FROM_BEGINNING);
    }
    else
    {
        CTOS_FileSeek(ulHandle ,position ,d_SEEK_FROM_EOF);
    }
    CTOS_Delay(50);
    if(CTOS_FileWrite ( ulHandle, data, strlen(data))==d_OK)
    {
        CTOS_FileClose ( ulHandle );
        return TRUE;
    }
    CTOS_Delay(50);
    CTOS_FileClose ( ulHandle );
    return FALSE;
}
BOOL balExist(BYTE *fileName)
{
    ULONG ulFileSize = 0, ret;
    ret=CTOS_FileGetSize(fileName,&ulFileSize);
    if(ret!=d_OK)
        return FALSE;
    return TRUE;
}
BOOL balMake(BYTE *fileName)
{
    ULONG ulHandle, ret;
    ret=CTOS_FileOpen(fileName , d_STORAGE_FLASH , &ulHandle);
    if(ret!=d_OK)
            return FALSE; 
     
     CTOS_FileClose(ulHandle);
    
}
int balGet(BYTE *fileName,BYTE *data)
{
    ULONG ulFileSize = 0, ulHandle, ret;
    ULONG dataLen=4;
//    BYTE data[5];
    int cardVal;
    cardVal=-1;
    memset(data,0,sizeof(data));
    ret=CTOS_FileGetSize(fileName,&ulFileSize);
    if(ret!=d_OK)
    {
        return cardVal;
    }
    if(CTOS_FileReopen(fileName ,d_STORAGE_FLASH ,&ulHandle)!=d_OK)
    {
 //       testData("open problem");
        return cardVal;
    }
    CTOS_FileSeek(ulHandle ,0 ,d_SEEK_FROM_BEGINNING);
    if(CTOS_FileRead ( ulHandle, data, &dataLen )==d_OK)
    {
        data[(int)dataLen]=0x00;
        CTOS_FileClose ( ulHandle );
//        testData(data);
        cardVal=StrToInt(data);
        return cardVal;
    }
    else
    {
        CTOS_FileClose ( ulHandle );
 //       testData("read fail");
        return cardVal;
    }
}
BOOL getmem(BYTE *fileName,BYTE data[],ULONG dataLen,int position,BOOL begin)
{
    ULONG ulFileSize = 0, ulHandle, ret;
    memset(data,0,sizeof(data));
    ret=CTOS_FileGetSize(fileName,&ulFileSize);
    if(ret!=d_OK)
    {
        return FALSE;
    }
    if(CTOS_FileReopen(fileName ,d_STORAGE_FLASH ,&ulHandle)!=d_OK)
        return FALSE;
    if(begin==TRUE)
    {
        CTOS_FileSeek(ulHandle ,position ,d_SEEK_FROM_BEGINNING);
    }
    else
    {
        CTOS_FileSeek(ulHandle ,position ,d_SEEK_FROM_EOF);
    }
    if(CTOS_FileRead ( ulHandle, data, &dataLen )==d_OK)
    {
        data[(int)dataLen]=0x00;
        CTOS_FileClose ( ulHandle );
        return TRUE;
    }
    else
    {
        CTOS_FileClose ( ulHandle );
        return FALSE;
    }
}
BOOL getTran(int offset, STR tranData[])
{
    STR fileName[10];
    ULONG ulFileSize = 128;
    strcpy(fileName,"trans");
    if(getmem(fileName,tranData,ulFileSize,offset,TRUE)==FALSE)
        return FALSE;
    return TRUE;
}
BOOL stoMerchant(STR dispMerch[])
{
    STR file[10];
    strcpy(file,"merchant");
    if(stomem(file,dispMerch,0,TRUE))
        return TRUE;
    return FALSE;
}
BOOL getMerchant(STR dispMerch[])
{
    STR fileName[10];
    ULONG ulFileSize = 16;
    strcpy(fileName,"merchant");
    if(getmem(fileName,dispMerch,ulFileSize,0,TRUE))
    {
        dispMerch[ulFileSize]=0x00;
        return TRUE;
    }
    return FALSE;
}
BOOL addToTotalP(int amount)
{
    int balance;
    balance=getTotalP();
    if(balance<0)
        return FALSE;
    balance=balance+amount;
    if(stoTotalP(balance)!=TRUE)
        return FALSE;
    return TRUE;
}
BOOL addToTotalC(int amount)
{
    int balance;
    balance=getTotalC();
    if(balance<0)
        return FALSE;
    balance=balance+amount;
    if(stoTotalC(balance)!=TRUE)
        return FALSE;
    return TRUE;
}
BOOL stoTotalP(int total)
{
    STR file[10];
    STR totAscii[10];
    strcpy(file,"totalP");
    memset(totAscii,0,sizeof(totAscii));
    sprintf(totAscii,"%06d",total);
    //intToAscii(total,totAscii,6);
    if(stomem(file,totAscii,0,TRUE))
        return TRUE;
    return FALSE;
}
BOOL stoTotalC(int total)
{
    STR file[10];
    STR totAscii[10];
    strcpy(file,"totalC");
    memset(totAscii,0,sizeof(totAscii));
    sprintf(totAscii,"%06d",total);
    //intToAscii(total,totAscii,6);
    if(stomem(file,totAscii,0,TRUE))
        return TRUE;
    return FALSE;
}
int getTotalP()
{
    STR file[10];
    STR totAscii[10];
    memset(totAscii,0,sizeof(totAscii));
    strcpy(file,"totalP");
    ULONG ulFileSize = 16;
    if(getmem(file,totAscii,ulFileSize,0,TRUE))
    {
        return StrToInt(totAscii);
    }
    return 0;
}
int getTotalC()
{
    STR file[10];
    STR totAscii[10];
    memset(totAscii,0,sizeof(totAscii));
    strcpy(file,"totalC");
    ULONG ulFileSize = 16;
    if(getmem(file,totAscii,ulFileSize,0,TRUE))
    {
        return StrToInt(totAscii);
    }
    return 0;
}
BOOL stoSettings()
{
    STR file[10];
    STR data[8];
    strcpy(file,"settings");
    if(doUpload==TRUE)
        data[0]='Y';
    else
        data[0]='N';
    if(gprsLan==TRUE)
        data[1]='G';
    else
        data[1]='L'; 
    data[2]=0x00;
    if(stomem(file,data,0,TRUE))
        return TRUE;
    return FALSE;
}
BOOL stoConfig()
{
    strcpy(sClientName,"Bubbly 2016");
    STR file[10];
    STR sTemp[40];
    strcpy(file,"config");
    
    if(!stomem(file,sClientName,0,TRUE))
        return FALSE;
   if(!stomem(file,sMerchantName,30,TRUE))
        return FALSE;
   if(!stomem(file,sDisp,60,TRUE))
        return FALSE;
   if(!stomem(file,sHost,80,TRUE))
        return FALSE;
   if(!stomem(file,sSchema,110,TRUE))
        return FALSE;
   if(!stomem(file,sTableHdr,140,TRUE))
        return FALSE;
   for(i=0;i<4;i++)
    {
        sTemp[i]=sCountry[i];
        sTemp[i+4]=sSystem[i];
        sTemp[i+8]=sClient[i];
        sTemp[i+12]=sKey[i];
    } 
    sTemp[16]=0x00;
    if(!stomem(file,sTemp,160,TRUE))
        return FALSE;
    
    if(sMerchCard)
        sTemp[0]='Y';
    else
        sTemp[0]='N';
   sTemp[1]=sCardType[0];        
   sTemp[2]=sTermType[0];        
   sTemp[3]=sUpload[0];             
   sTemp[4]=sCrDr[0];
   sTemp[5]='N';
   if(sRcptA)
       sTemp[5]='Y';
   sTemp[6]='N';
   if(sRcptB)
       sTemp[6]='Y';
   sTemp[7]='D';
   if(sEnable)
       sTemp[7]='E';
   sTemp[8]=sLoyalty[0];             
   sTemp[9]=sLoyalty[1];
   sTemp[10]=0x00;
   if(!stomem(file,sTemp,180,TRUE))
        return FALSE;
    return TRUE;
}
BOOL getConfig()
{
    STR file[10];
    STR sTemp[30];
    ULONG ulFileSize;
    strcpy(file,"config");
    memset(sClientName,0,sizeof(sClientName));
    memset(sMerchantName,0,sizeof(sMerchantName));
    memset(sDisp,0,sizeof(sDisp));
    memset(sHost,0,sizeof(sHost));
    memset(sSchema,0,sizeof(sSchema));
    memset(sTableHdr,0,sizeof(sTableHdr));
    memset(sTemp,0,sizeof(sTemp));

    if(!getmem(file,sClientName,30,0,TRUE))
        return FALSE;
//    testData(sClientName);
    if(!getmem(file,sMerchantName,30,30,TRUE))
        return FALSE;
    if(!getmem(file,sDisp,20,60,TRUE))
        return FALSE;
    
//    if(!getmem(file,sHost,30,80,TRUE))
//        return FALSE;
//    testData(sHost);
//    if(!getmem(file,sSchema,30,110,TRUE))
//        return FALSE;
//    testData(sSchema);
    if(!getmem(file,sTableHdr,20,140,TRUE))
        return FALSE;
//    testData(sTableHdr);
    if(!getmem(file,sTemp,20,160,TRUE))
        return FALSE;
    for(i=0;i<4;i++)
    {
        sCountry[i]=sTemp[i];
        sSystem[i]=sTemp[i+4];
        sClient[i]=sTemp[i+8];
        sKey[i]=sTemp[i+12];
    }
    sCountry[4]=0x00;
    sSystem[4]=0x00;
    sClient[4]=0x00;
    sKey[4]=0x00;
    if(!getmem(file,sTemp,10,180,TRUE))
        return FALSE;
    sMerchCard=FALSE;
    if(sTemp[0]=='Y')
        sMerchCard=TRUE;
    sRcptA=FALSE;
    if(sTemp[5]=='Y')
        sRcptA=TRUE;
    sRcptB=FALSE;
    if(sTemp[6]=='Y')
        sRcptB=TRUE;
    sCardType[0]=sTemp[1];      //      P/C/E
    sCardType[1]=0x00;
    sTermType[0]=sTemp[2];      //      T/C
    sTermType[1]=0x00;
    sUpload[0]=sTemp[3];        //      A/M
    sUpload[1]=0x00;
    sCrDr[0]=sTemp[4];          //      C/D
    sCrDr[1]=0x00;
    sEnable=FALSE;
    if(sTemp[7]=='E')
        sEnable=TRUE;
    sLoyalty[0]=sTemp[8];
    sLoyalty[1]=sTemp[9];
    sLoyalty[2]=0x00;
//    testData(sCrDr);
    return TRUE;
}

BOOL getSettings()
{
    STR file[10];
    STR data[9];
    ULONG ulFileSize = 16;
    strcpy(file,"settings");
    if(getmem(file,data,ulFileSize,0,TRUE))
    {
        data[2]=0x00;
        if(data[0]=='Y')
            doUpload=TRUE;
        else
            doUpload=FALSE;
        if(data[1]=='G')
            gprsLan=TRUE;
        else
            gprsLan=FALSE;
        return TRUE;
    }
    return FALSE;
}

BOOL deleteTran()
{
    STR file[10];
    strcpy(file,"total");
    if(recreate(file)!=TRUE)
    {
        return FALSE;
    }
    
    CTOS_Delay(50);
    strcpy(file,"trans");
    if(recreate(file)!=TRUE)
    {
        return FALSE;
    }
    CTOS_Delay(50);
    if(stoTotalP(0)!=TRUE)
    {
        return FALSE;
    }
    CTOS_Delay(50);
    if(stoTotalC(0)!=TRUE)
    {
        return FALSE;
    }return TRUE;
}
BOOL deleteConf()
{
    STR file[10];
    strcpy(file,"config");
    if(recreate(file)!=TRUE)
    {
        return FALSE;
    }
    return TRUE;
}
BOOL stoTran(STR tType[],STR tRef[],STR tGross[],STR tDisc[],STR tTip[],STR tNett[],STR tBal[],STR tSer0[],STR tSer1[],STR tId0[],STR tId1[],STR tPurse[],STR tLpoints[],STR tLbal[],BOOL sent)
{
    STR trans[140];
    STR file[10];
    strcpy(file,"trans");
    i=strlen(tId0);
    int b=16-i;
    for(i=0;i<b;i++)
    {
        strcat(tId0," ");
    }

    strcpy(trans,tType);        //2 0
    strcat(trans,tranTime);     //6 2
    strcat(trans,tranDate);     //6 8
    strcat(trans,tRef);         //8 14
    strcat(trans,tGross);       //8 22
    strcat(trans,tDisc);        //8 30
    strcat(trans,tTip);        //8 30
    strcat(trans,tNett);        //8 38
    strcat(trans,tBal);         //8 46
    strcat(trans,tPurse);       //2 54
    strcat(trans,tSer0);        //8 56
    strcat(trans,tSer1);        //8 64
    strcat(trans,tId0);         //16 72
    strcat(trans,tId1);         //16 88
    strcat(trans,tLpoints);     //6 104
    strcat(trans,tLbal);        //8 110  ..118
    
    if(sent==TRUE)
        strcat(trans,"0");
    else
        strcat(trans,"F");
    strcat(trans,"#");
    
//    testData(trans);
   
    if(stomem(file,trans,0,FALSE))
        return TRUE;
    return FALSE;
}
int getTranCountSent()
{
    STR tran[130];
    int count,cntt;
    count=0;
    cntt=0;
    while(getTran(count,tran)==TRUE)
    {
        if(strlen(tran)==128)
        {
            if((tran[126]=='0')&&(tran[127]=='#'))
                cntt++;
        }
        else
        {
            break;
        }
        count=count+128;
    }
    return cntt;
}
int getTranCountPend()
{
    STR tran[130];
    int count,cntt;
    count=0;
    cntt=0;
    while(getTran(count,tran)==TRUE)
    {
        if(strlen(tran)==128)
        {
            if((tran[126]=='F')&&(tran[127]=='#'))
                cntt++;
        }
        else
        {
            break;
        }
        count=count+128;
    }
    return cntt;
}
int getTranCountAll()
{
    STR tran[130];
    int count,cntt;
    count=0;
    cntt=0;
    while(getTran(count,tran)==TRUE)
    {
        if(strlen(tran)==128)
        {
            if(tran[127]=='#')
                cntt++;
        }
        else
        {
            break;
        }
        count=count+128;
    }
    return cntt;
}
//
//      General Routines
//

void getSerial()
{
    if(CTOS_GetFactorySN(baBuf)==d_OK)    
    {
        for (i=0;i<10;i++)
        {
            termSerial[i]=baBuf[i+5];
        }
        termSerial[10]=0x00;
    }
    else
    {
        strcpy(termSerial,"??????????");
    }
}
BYTE checkBattery()
{
    BYTE bPercent[1];
    if(CTOS_BatteryGetCapacity (bPercent)==d_OK)
    {
        return bPercent[0];
    }
    return 0x00;
}
void showError(STR comment[])
{
	CTOS_LCDTClearDisplay();	
	PrintStringCenter(4,comment);
        CTOS_Beep();
	CTOS_Delay(1200);
}
int StrToInt(STR *sStr)
{
    int temp=0;
    int sum=0;
    j = strlen(sStr);
    for(i=0; i<j; i++)
    {
        temp=(sStr[i]-48)*PowTen(j-(i+1));
        sum=sum+temp;
    }
    return sum;
}
ULONG PowTen(USHORT usP)
{
    if(usP==0)
        return 1;
    if(usP==1)
        return 10;
    if(usP==2)
        return 100;
    if(usP==3)
        return 1000;
    if(usP==4)
        return 10000;
    if(usP==5)
        return 100000;
}
STR intToAscii(int input,STR output[],int length)
{
	memset(sBuf,0,sizeof(sBuf));
	sprintf(sBuf,"%ld",input);
	j = strlen(sBuf);
	for(ii = 0 ;ii< j; ii++)
	{
		output[ii+length-j] = sBuf[ii];
	}
	if(j<length)
	{
		for(ii = 0 ;ii< length-j; ii++)
		{
			output[ii] = 0x30;
		}
	}
	output[length] = 0x00;
}
void doPrint(STR xName[],STR xNumber[],STR xStoreName[],STR xCentre[],STR xAmount[],STR xBalance[],BOOL client)
{
	STR strBal[34];
	STR timedate[34];
	STR strAmt[34];
	STR strName[34];
	STR strNumber[34];
	STR strStore[34];
	STR strCentre[34];
	STR termNo[34];
	STR strTotal[34];
	STR hdr1[34];
        STR type[34];
	
 	strcpy(hdr1,"          ");
	strcat(hdr1,sClientName);
	strcat(hdr1,"     ");
	strcpy(timedate,"  ");
	strcat(timedate,dispTime);
	strcat(timedate,"            ");
	strcat(timedate,dispDate);
	strcat(timedate,"  ");
	strcpy(strStore,"Merchant:     ");
	strcat(strStore,xStoreName);
	strcpy(strCentre,"Cost Centre:  [");
	strcat(strCentre,xCentre);
        strcat(strCentre,"]");
	strcpy(termNo,"Terminal No:  ");
	strcat(termNo,termSerial);
	strcpy(strAmt,"Amount:     ");
	strcat(strAmt,xAmount);
	strcpy(strBal,"Balance:    ");
	strcat(strBal,xBalance);

	CTOS_PrinterPutString(hdr1);
	CTOS_PrinterPutString("\n");
	CTOS_PrinterPutString(timedate);
	CTOS_PrinterPutString("\n");
	CTOS_PrinterPutString(strStore);
	CTOS_PrinterPutString(strCentre);
	CTOS_PrinterPutString(termNo);
	CTOS_PrinterPutString("\n");
        if(sCrDr[0]=='C')
            CTOS_PrinterPutString("CREDIT");
        else
            CTOS_PrinterPutString("SALE");
	CTOS_PrinterPutString("\n");
        CTOS_PrinterPutString(strAmt);
	CTOS_PrinterPutString(strTotal);
	
	if((client))
	{
		CTOS_PrinterPutString(strBal);
	}
	CTOS_PrinterPutString("\n");
		
	if(!client)
	{
		CTOS_PrinterPutString("\n");
		CTOS_PrinterPutString("\n");
		CTOS_PrinterPutString("Signature: _____________________");
		CTOS_PrinterPutString("\n");
                CTOS_PrinterPutString("*** MERCHANT COPY ***");
		
	}	
	CTOS_PrinterPutString("\n");
	CTOS_PrinterPutString("\n");
	CTOS_PrinterPutString("\n");
}

void testData(STR qData[])
{
	int	ix,iz;
	STR test[26];
	for (iz=0; iz<strlen(qData); iz=iz+25)
	{
		for(ix=0; ix<25; ix++)
		{
			test[ix] = qData[iz+ix];
		}
		test[26] = 0x00;
		CTOS_PrinterPutString(test);
	}
	CTOS_PrinterPutString("\n");
	CTOS_PrinterPutString("\n");
	CTOS_PrinterPutString("\n");
}
STR scrnFormat(STR input[],STR output[],int length,BOOL decimal,BOOL blank)
{
	STR temp[8];
	for(ii = 0 ;ii< length; ii++)
	{
		temp[ii] = input[ii];
	}
	if(decimal)
	{
		for(ii = 0 ;ii< length-2; ii++)
		{
			output[ii] = temp[ii];
		}
		output[length-2] = '.';
		output[length-1] = temp[length-2];
		output[length] = temp[length-1];
		output[length+1] = 0x00;
	}
	if(!decimal)
	{
		for(ii = 0 ;ii< length; ii++)
		{
			output[ii] = temp[ii];
		}
		output[length] = 0x00;
	}	
	if(blank)
	{
		for(ii = 0; ii <length-3; ii++)
		{
			if(output[ii] == 0x30)
			{
				output[ii] = 0x20;
			}
			else
			{
				break;
			}
		}
	}
        if(decimal)
        {
        if(output[length-1]==0x20)
            output[length-1]=0x30;
         if(output[length-3]==0x20)
            output[length-3]=0x30;
        }
}

void doUploadMode()
{
    getSettings();
    CTOS_LCDTClearDisplay();
    PrintStringCenter(1,"UPLOAD MODE");
    if(doUpload==TRUE)
    {
        CTOS_LCDTPrintXY(1,3,"Upload: Auto");
    }
    else
    {
        CTOS_LCDTPrintXY(1,3,"Upload: Manual");
    }
    CTOS_LCDTPrintXY(1,5,"1 Automatic");
    CTOS_LCDTPrintXY(1,6,"2 Manual");
    key='0';
    //CTOS_KBDGet(&key);
    while((key!=d_KBD_CANCEL)&&(key!='1')&&(key!='2')&&(key!=d_KBD_ENTER))
    {
        CTOS_KBDGet(&key);
        if(key==d_KBD_CANCEL)
            break;
        if(key==d_KBD_ENTER)
            break;
        if(key=='1')
        {
            doUpload=TRUE;
	}
        if(key=='2')
        {
            doUpload=FALSE;
	}
	if((key=='1')||(key=='2'))
	{
            stoSettings();
            showError("Upload Updated");
            break;
	}
    }
}
void doUploadType()
{
    getSettings();
    CTOS_LCDTClearDisplay();
    PrintStringCenter(1,"UPLOAD TYPE");
    if(gprsLan==TRUE)
    {
        CTOS_LCDTPrintXY(1,3,"Upload: GPRS");
    }
    else
    {
        CTOS_LCDTPrintXY(1,3,"Upload: LAN");
    }
    CTOS_LCDTPrintXY(1,5,"1 GPRS");
    CTOS_LCDTPrintXY(1,6,"2 LAN");
    key='0';
    while((key!=d_KBD_CANCEL)&&(key!='1')&&(key!='2')&&(key!=d_KBD_ENTER))
    {
        CTOS_KBDGet(&key);
        if(key==d_KBD_CANCEL)
            break;
        if(key==d_KBD_ENTER)
            break;
        if(key=='1')
        {
            gprsLan=TRUE;
	}
        if(key=='2')
        {
            gprsLan=FALSE;
	}
	if((key=='1')||(key=='2'))
	{
            stoSettings();
            showError("Type Updated");
            break;
	}
    }
}
//
//      Printer Routines
//
void doTotalsScrn()
{
    STR testP[10];
    STR testC[10];
    STR testA[10];
    int totalP;
    int totalC;
    int totalAll;

    CTOS_LCDForeGndColor(0x0000ff);
    CTOS_LCDTPrintXY(1,1," Total Vouchers ");
    CTOS_LCDForeGndColor(0x000000);
    totalP=getTotalP();
    totalC=getTotalC();
    
    if(totalP==-1)
        totalP=0;
    if(totalC==-1)
        totalC=0;
    totalAll=totalP+totalC;
    sprintf(testC,"% 6d",totalC);
    CTOS_LCDTPrintXY(1,4,"Comp:           ");
    CTOS_LCDTPrintXY(8,4,testC);
    sprintf(testP,"% 6d",totalP);
    CTOS_LCDTPrintXY(1,5,"Bought:         ");
    CTOS_LCDTPrintXY(8,5,testP);
    sprintf(testA,"% 6d",totalAll);
    CTOS_LCDTPrintXY(1,6,"TOTAL:          ");
    CTOS_LCDTPrintXY(8,6,testA);
    
    CTOS_LCDForeGndColor(0x21cd3a);
    CTOS_LCDTPrintXY(13,8,"[OK]");
    CTOS_LCDForeGndColor(0x000000);
    while(key!=d_KBD_ENTER)
    {
        CTOS_KBDGet(&key);
        if(key==d_KBD_ENTER)
            break;
    }
}
void doTotals()
{
    getSerial();
    STR timedate[34];
    STR totdispP[34];
    STR totdispC[34];
    STR totdispA[34];
    STR tot[10];
    STR serial[34];
    STR merch[34];
    STR testP[10];
    STR testC[10];
    STR testA[10];
    int totalP;
    int totalC;
    int totalA;
    int mLen;
    if(CTOS_PrinterStatus()==d_OK)
    {
        totalP=getTotalP();
        totalC=getTotalC();
        totalA=totalP+totalC;
        if((totalP>-1)&&(totalC>-1))
        {
            if(totalP==0)
            {
                strcpy(testP,"     0");
            }
            else
            {
                sprintf(testP,"% 6d",totalP);
            }
            if(totalC==0)
            {
                strcpy(testC,"     0");
            }
            else
            {
               sprintf(testC,"% 6d",totalC);
            }
            if(totalA==0)
            {
                strcpy(testA,"     0");
            }
            else
            {
               sprintf(testA,"% 6d",totalA);
            }
            
            mLen=(32-strlen(sDisp))/2;
            strcpy(merch," ");
            for(ii=0;ii<mLen-2;ii++)
            {
                strcat(merch," ");
            }
            strcat(merch,sDisp);
            getTimeDate();
            strcpy(timedate,"  ");
            strcat(timedate,dispTime);
            strcat(timedate,"            ");
            strcat(timedate,dispDate);
            strcat(timedate,"  ");
            strcpy(totdispP,"    Total Bought: ");
            strcat(totdispP,testP);
            strcpy(totdispC,"    Total Comp:   ");
            strcat(totdispC,testC);
            strcpy(totdispA,"    TOTAL:        ");
            strcat(totdispA,testA);
            
            strcpy(serial,"Terminal: ");
            strcat(serial,termSerial);
            CTOS_PrinterPutString(merch);
            CTOS_PrinterPutString("\n");
            CTOS_PrinterPutString("     Transaction  Total");
            CTOS_PrinterPutString("\n");
            CTOS_PrinterPutString(timedate);
            CTOS_PrinterPutString("\n");
            CTOS_PrinterPutString(totdispC);
            CTOS_PrinterPutString(totdispP);
            CTOS_PrinterPutString(totdispA);
            CTOS_PrinterPutString("\n");
            CTOS_PrinterPutString(serial);
            CTOS_PrinterPutString("\n");
            CTOS_PrinterPutString("\n");
            CTOS_PrinterPutString("\n");
        }
        else
        {
            showError("Report Error");
        }
    }
    else
    {
        showError("Printer Error");
    }
}
void doReport()
{
    getSerial();
    STR timedate[34];
    STR disp1[34];
    STR disp2[34];
    STR tot[10];
    STR totdisp[34];
    STR serial[34];
    STR merch[34];
    STR tran[50];
    STR trana[10];
    STR tranb[10];
    STR sentTrans[34];
    STR pendTrans[34];
    STR tempID[20];
    STR packTemp[130];
    
    int total,amt,mLen,count,sent,pend,tt;
    if(CTOS_PrinterStatus()==d_OK)
    {
        total=0;
        mLen=(32-strlen(merchant))/2;
        strcpy(merch," ");
        for(ii=0;ii<mLen-2;ii++)
        {
            strcat(merch," ");
        }
        strcat(merch,merchant);
        getTimeDate();
        strcpy(timedate,"  ");
        strcat(timedate,dispTime);
        strcat(timedate,"            ");
        strcat(timedate,dispDate);
        strcat(timedate,"  ");
        strcpy(serial,"Terminal: ");
        strcat(serial,termSerial);
        CTOS_PrinterPutString(merch);
        CTOS_PrinterPutString("\n");
        CTOS_PrinterPutString("    Transaction  Listing");
        CTOS_PrinterPutString("\n");
        CTOS_PrinterPutString(timedate);
        CTOS_PrinterPutString("\n");
        count=0;
        sent=0;
        pend=0;
        while(getTran(count,tran)==TRUE)
        {
            if(strlen(tran)==128)
            {
                if(tran[126]=='0')
                    sent++;
                else
                    pend++;
                
                packSub(80,16,tran,tempID);
                packSub(0,62,tran,packTemp);
                
                for(ii=0;ii<16;ii++)
                {
                    disp1[ii]=tempID[ii];
                }
                
                disp1[16]=' ';
                disp1[17]=' ';
                disp1[18]=tran[2];
                disp1[19]=tran[3];
                disp1[20]=':';
                disp1[21]=tran[4];
                disp1[22]=tran[5];
                disp1[23]=' ';
                disp1[24]=tran[8];
                disp1[25]=tran[9];
                disp1[26]='/';
                disp1[27]=tran[10];
                disp1[28]=tran[11];
                disp1[29]='/';
                disp1[30]=tran[12];
                disp1[31]=tran[13];
                disp1[32]=0x00;
                
                for(ii=0;ii<8;ii++)
                {
                    trana[ii]=tran[ii+22];
                }
                trana[8]=0x00;
                tt=StrToInt(trana);
                sprintf(tranb,"% 7d",tt);
                if(sTermType[0]=='C')
                scrnFormat(tranb,trana,7,TRUE,TRUE);
                else
                scrnFormat(tranb,trana,7,FALSE,TRUE);
                    
                if(packTemp[0]=='1')
                    strcpy(disp2,"Credit:  ");
                else
                    strcpy(disp2,"Debit:   ");
                    
                strcat(disp2,trana);
                CTOS_PrinterPutString(disp1);
                CTOS_PrinterPutString(disp2);
                CTOS_PrinterPutString("\n");

                count=count+128;
                total++;
            }
            else
                break;
        }
        strcpy(totdisp,"Total Transactions:   ");
        sprintf(tot,"% 4d",total);
        strcat(totdisp,tot);
        
        strcpy(sentTrans,"Sent Transactions:    ");
        sprintf(tot,"% 4d",sent);
        strcat(sentTrans,tot);
        
        strcpy(pendTrans,"Pending Transactions: ");
        sprintf(tot,"% 4d",pend);
        strcat(pendTrans,tot);
        
        CTOS_PrinterPutString(pendTrans);
        CTOS_PrinterPutString(sentTrans);
        CTOS_PrinterPutString(totdisp);
        CTOS_PrinterPutString("\n");
        CTOS_PrinterPutString(serial);
        CTOS_PrinterPutString("\n");
        CTOS_PrinterPutString("\n");
        CTOS_PrinterPutString("\n");
    }
    else
    {
        showError("Printer Error");
    }
}
BOOL Check_Card_state(BYTE *ATQA_f, BYTE *SAK_f, BYTE *SN, BYTE *SN_Len)
{

if(CTOS_CLTypeAActiveFromIdle(0, ATQA_f, SAK_f,SN, SN_Len)!=d_OK)
{
    return FALSE;
}
return TRUE;
}
BOOL Authenticate(BYTE Block, BYTE KeyType, BYTE Key[6], BYTE *sn)
{
BYTE i;
BYTE KeyBuf=0x00;
BYTE AuthenKey [12];
memset(AuthenKey,0x00,sizeof(AuthenKey));
for(i=0;i<6;i++)
{
KeyBuf=Key[i];
AuthenKey[i*2]=(KeyBuf^0xF0)&0xF0;
AuthenKey[i*2]|= (KeyBuf>>4);
AuthenKey[i*2+1]=((KeyBuf^0x0F)<<4);
AuthenKey[i*2+1]|= (KeyBuf&0x0F);
}
if(CTOS_MifareLOADKEY(AuthenKey)!=d_OK)
return FALSE;
if(CTOS_MifareAUTH(KeyType, Block, sn)!=d_OK)
return FALSE;
return TRUE;
}
BOOL Decrease_Part(UCHAR bBlockNr,UCHAR* bValue)
{
if(CTOS_MifareDECREASE(bBlockNr,bValue)!=d_OK)
return FALSE;
if(CTOS_MifareTRANSFER(bBlockNr)!=d_OK)
return FALSE;
return TRUE;
}
BOOL Increase_Part(UCHAR bBlockNr,UCHAR* bValue)
{
if(CTOS_MifareINCREASE(bBlockNr,bValue)!=d_OK)
return FALSE;
if(CTOS_MifareTRANSFER(bBlockNr)!=d_OK)
return FALSE;
return TRUE;
}
STR nibble(int in)
{
    if(in<10)
        return in+0x30;
    if(in==0x0a)
        return 0x41;
    if(in==0x0b)
        return 0x42;
    if(in==0x0c)
        return 0x43;
    if(in==0x0d)
        return 0x44;
    if(in==0x0e)
        return 0x45;
    if(in==0x0f)
        return 0x46;
}
int StartTrans(ULONG ulValue,STR cardNumberX[],STR cardID[],STR low[]) 
{
    BYTE CSN_Len=0;
BYTE ATQA[2];
BYTE SAK[10];
UCHAR ValueBuf [17];
UCHAR DataeBuf [16];
int counter;
UCHAR DataBlock=8;
ULONG Money;
UCHAR str[5];
UCHAR Card_state=0;
BYTE Key[6];
BYTE KeyT[6];
BYTE CSN[10];
BYTE KeyType=0x61;
STR cardBalance[8];
STR cardIDX[17];
STR cardSys[17];
STR word1X[17];
STR word2[33];
STR word2X[17];

        memset(low,0,sizeof(low));      
    CTOS_CLInit(); 
    CTOS_CLPowerOn();
    Money=-1;
    while(Check_Card_state(ATQA, SAK, CSN, &CSN_Len)!=TRUE)
    {
        CTOS_KBDHit(&key);
            if(key==d_KBD_CANCEL)
             {
                CTOS_CLPowerOff();
                return -1;
              }
    }
    
             /* 
           Key[0]=0x08;
           Key[1]=CSN[0];
           Key[2]=CSN[3];
           Key[3]=0x08;
           Key[4]=CSN[2];
           Key[5]=CSN[1];
           */
           //currentCardKey
           Key[0]=currentCardKey[0];
           Key[1]=CSN[0];
           Key[2]=CSN[3];
           Key[3]=currentCardKey[1];
           Key[4]=CSN[2];
           Key[5]=CSN[1];
           
       if(!Authenticate(1, KeyType, Key, CSN))
    {
        CTOS_CLPowerOff();
        return -1;
    }
    if(CTOS_MifareREADBLOCK(1, ValueBuf)!=d_OK)
    {
        CTOS_CLPowerOff();
        return -1;
    }
     
    if(ValueBuf[0]!=0x00)
    {
        CTOS_CLPowerOff();
        return -1;
    }
    if(ValueBuf[1]!=0x27)
    {
        CTOS_CLPowerOff();
        return -1;
    }
    if(ValueBuf[2]!=0x00)
    {
        CTOS_CLPowerOff();
        return -1;
    }
    if(ValueBuf[3]!=0x04)
    {
        CTOS_CLPowerOff();
        return -1;
    }
    if(ValueBuf[8]!=0x58)
    {
        CTOS_CLPowerOff();
        return -1;
    }
    if(ValueBuf[9]!=0x12)
    {
        CTOS_CLPowerOff();
        return -1;
    }

    cardID[0]=((ValueBuf[14]&0xf0)/16)+0x30;
    if(cardID[0]>0x39)
        cardID[0]=cardID[0]+7;
    cardID[1]=(ValueBuf[14]&0x0f)+0x30;
    if(cardID[1]>0x39)
        cardID[1]=cardID[1]+7;
    cardID[2]=((ValueBuf[15]&0xf0)/16)+0x30;
    if(cardID[2]>0x39)
        cardID[2]=cardID[2]+7;
    cardID[3]=(ValueBuf[15]&0x0f)+0x30;
    if(cardID[3]>0x39)
        cardID[3]=cardID[3]+7;
    cardID[4]=0x00;
     
    cardNumberX[0]=nibble((CSN[0]&0xf0)/16);
    cardNumberX[1]=nibble(CSN[0]&0x0f);
    cardNumberX[2]=nibble((CSN[1]&0xf0)/16);
    cardNumberX[3]=nibble(CSN[1]&0x0f);
    cardNumberX[4]=nibble((CSN[2]&0xf0)/16);
    cardNumberX[5]=nibble(CSN[2]&0x0f);
    cardNumberX[6]=nibble((CSN[3]&0xf0)/16);
    cardNumberX[7]=nibble(CSN[3]&0x0f);
    cardNumberX[8]=0x00;
    
         if(!Authenticate(2, KeyType, Key, CSN))
        {    CTOS_CLPowerOff();
         return -1;
        }
        if(CTOS_MifareREADBLOCK(2, ValueBuf)!=d_OK)
        {
            CTOS_CLPowerOff();
            return -1;
        }
        
   Money=(ValueBuf[3]<<24)+(ValueBuf[2]<<16)+(ValueBuf[1]<<8)+(ValueBuf[0]);
   
    
    if(ulValue==0)
    {
        CTOS_CLPowerOff();
        return Money;
    }
   if(sCrDr[0]=='D')
   {
       if(ulValue>Money)
        {
           sprintf(low,"%1d",Money);
           CTOS_CLPowerOff();
            return -1; 
        }
   }
   else
       
   {
       if((cardID[0]!='0')||(cardID[1]!='0')||(cardID[2]!='0')||(cardID[3]!='0'))
       {
           showError("NO! Comp Card");
            CTOS_CLPowerOff();
            return -1;
       }
   }
        if(sCrDr[0]=='D')
        {
            if(Decrease_Part(2, (UCHAR*)&ulValue)!=TRUE)
           {
            CTOS_CLPowerOff();
                return -1;
            }
        }
        else
        {
            if(Increase_Part(2, (UCHAR*)&ulValue)!=TRUE)
           {
            CTOS_CLPowerOff();
                return -1;
            }
        }    
        if(CTOS_MifareREADBLOCK(2, ValueBuf)!=d_OK)
        {
            CTOS_CLPowerOff();
         return -1;
        }

   Money=(ValueBuf[3]<<24)+(ValueBuf[2]<<16)+(ValueBuf[1]<<8)+(ValueBuf[0]);
           KeyT[0]=0xff;
           KeyT[1]=0xff;
           KeyT[2]=0xff;
           KeyT[3]=0xff;
           KeyT[4]=0xff;
           KeyT[5]=0xff;

   strcpy(word1X,sDisp);         //16
   strcpy(word2,tranTime);      //6
   strcat(word2,tranDate);      //6
   strcat(word2,termSerial);    //10
   if(sCrDr[0]=='D')
       strcat(word2,"00");
   else
       strcat(word2,"01");      //2
   sprintf(str,"%04d",ulValue);
   strcat(word2,str);           //4
   sprintf(str,"%04d",Money);
   strcat(word2,str);           //4
   
   for (i=0;i<16;i++)
   {
       word2X[i]=((word2[i*2]&0x0f)*16)+(word2[(i*2)+1]&0x0f);
   }
   counter=getPlace(KeyT,CSN);
   if(counter>=0)
   {
       if(writeLog(KeyT,CSN,counter,word1X,word2X))
       {
           stoPlace(KeyT,CSN,counter);
       }
   }
   //
   CTOS_CLPowerOff();
   return Money;
}
int getPlace(BYTE KeyX[],BYTE csnX[])
{
    UCHAR ValueBuf [17];

    if(!Authenticate(6, 0x60, KeyX, csnX))
    {
        return -1;
    }
    if(CTOS_MifareREADBLOCK(6, ValueBuf)!=d_OK)
    {
        return -1;
    }
    return ValueBuf[15];
}
int stoPlace(BYTE KeyX[],BYTE csnX[],int sto)
{
    UCHAR ValueBuf [17];

    if(!Authenticate(6, 0x60, KeyX, csnX))
    {
        return -1;
    }
    for (i=0;i<15;i++)
    {
        ValueBuf[i]=0x00;
    }
    sto++;
    if(sto>6)
        sto=0;
    ValueBuf[15]=sto;
    if(CTOS_MifareWRITEBLOCK(6, ValueBuf)!=d_OK)
    {
        return -1;
    }
    return sto;
}
BOOL writeLog(BYTE KeyX[],BYTE csnX[],int counter,BYTE word1[],BYTE word2[])
{
    UCHAR sector1;
    UCHAR sector2;
    if(counter>=0)
    {
        sector1=(counter+1)*4;
        sector2=sector1+1;
    }
    if(!Authenticate(sector1, 0x60, KeyX, csnX))
    {
        return FALSE;
    }
    if(CTOS_MifareWRITEBLOCK(sector1, word1)!=d_OK)
    {
        return FALSE;
    }
    if(CTOS_MifareWRITEBLOCK(sector2, word2)!=d_OK)
    {
        return FALSE;
    }
    return TRUE;
}
BOOL doLog(STR cardIDZ[],STR balZX[])
{
       BYTE CSN_Len=0;
BYTE ATQA[2];
BYTE SAK[10];
UCHAR ValueBuf [17];
UCHAR DataeBuf [16];
int counter;
UCHAR DataBlock=8;
ULONG Money;
UCHAR str[5];
UCHAR Card_state=0;
BYTE Key[6];
BYTE KeyT[6];
BYTE CSN[10];
BYTE KeyType=0x61;
STR cardBalance[8];
STR cardIDX[17];
STR cardSys[17];
STR word1X[17];
STR word2[33];
STR word2X[17];
   UCHAR sector1;
    UCHAR sector2;
    STR termName[17];
    STR termSer[11];
    STR transaction1[34];
    STR transaction2[34];
    STR temp[5];
    int amt;
    STR timedate[34];
    STR cardnum[34];
    STR cardbal[34];
    
    CTOS_CLInit(); 
    CTOS_CLPowerOn();
    Money=-1;
    while(Check_Card_state(ATQA, SAK, CSN, &CSN_Len)!=TRUE)
    {
        CTOS_KBDHit(&key);
            if(key==d_KBD_CANCEL)
             {
                CTOS_CLPowerOff();
                return -1;
              }
    }
           KeyT[0]=0xff;
           KeyT[1]=0xff;
           KeyT[2]=0xff;
           KeyT[3]=0xff;
           KeyT[4]=0xff;
           KeyT[5]=0xff;

           strcpy(cardnum,"Card:       ");
           strcat(cardnum,cardIDZ);
           strcpy(cardbal,"Balance: ");
           strcat(cardbal,balZX);
           
        CTOS_PrinterPutString("      Transaction Listing");
	CTOS_PrinterPutString("\n");
	CTOS_PrinterPutString(timedate);
	CTOS_PrinterPutString("\n");
	CTOS_PrinterPutString(cardnum);
	CTOS_PrinterPutString(cardbal);
	CTOS_PrinterPutString("\n");
        
    for (counter = 0; counter <7; counter++)
    {
        sector1=(counter+1)*4;
        sector2=sector1+1;
        if(!Authenticate(sector1, 0x60, KeyT, CSN))
        {
                CTOS_CLPowerOff();
                return FALSE;
        }
        if(CTOS_MifareREADBLOCK(sector1, ValueBuf)!=d_OK)
        {
            CTOS_CLPowerOff();
            return FALSE;
        }
        if(ValueBuf[0]==0x00)
            goto logdone;;
        
       for(i=0; i<16; i++)
        {
            termName[i]=ValueBuf[i];
        }
        termName[16]=0x00;
        if(CTOS_MifareREADBLOCK(sector2, ValueBuf)!=d_OK)
        {
            CTOS_CLPowerOff();
            return FALSE;
        }
        for(i=6; i<11; i++)
        {
            transaction1[(i-6)*2]=(((ValueBuf[i])&0xf0)/16)+0x30;
            transaction1[((i-6)*2)+1]=(ValueBuf[i]&0x0f)+0x30;
        }
        transaction1[11]=0x00;
        strcat(transaction1,"    ");
        strcat(transaction1,termName);
        transaction2[0]=(((ValueBuf[0])&0xf0)/16)+0x30;
        transaction2[1]=(ValueBuf[0]&0x0f)+0x30;
        transaction2[2]=':';
        transaction2[3]=(((ValueBuf[1])&0xf0)/16)+0x30;
        transaction2[4]=(ValueBuf[1]&0x0f)+0x30;
        transaction2[5]=' ';
        transaction2[6]=' ';
        transaction2[7]=(((ValueBuf[3])&0xf0)/16)+0x30;
        transaction2[8]=(ValueBuf[3]&0x0f)+0x30;
        transaction2[9]='/';
        transaction2[10]=(((ValueBuf[4])&0xf0)/16)+0x30;
        transaction2[11]=(ValueBuf[4]&0x0f)+0x30;
        transaction2[12]=' ';
        transaction2[13]=' ';
        if(ValueBuf[11]==0x01)
        {
            transaction2[14]='C';
            transaction2[15]='r';
        }
        else
        {
            transaction2[14]='D';
            transaction2[15]='r';
        }
        transaction2[16]=0x00;
        temp[0]=(((ValueBuf[12])&0xf0)/16)+0x30;
        temp[1]=(ValueBuf[12]&0x0f)+0x30;
        temp[2]=(((ValueBuf[13])&0xf0)/16)+0x30;
        temp[3]=(ValueBuf[13]&0x0f)+0x30;
        temp[4]=0x00;
        amt=StrToInt(temp);
        sprintf(temp,"% 4d",amt);
        strcat(transaction2,temp);
        strcat(transaction2,"  Bal");
        temp[0]=(((ValueBuf[14])&0xf0)/16)+0x30;
        temp[1]=(ValueBuf[14]&0x0f)+0x30;
        temp[2]=(((ValueBuf[15])&0xf0)/16)+0x30;
        temp[3]=(ValueBuf[15]&0x0f)+0x30;
        temp[4]=0x00;
        amt=StrToInt(temp);
        sprintf(temp,"% 4d",amt);
        strcat(transaction2,temp);
        
        CTOS_PrinterPutString(transaction1);
        CTOS_PrinterPutString(transaction2);
 	CTOS_PrinterPutString("\n");
    }
        logdone:

 	CTOS_PrinterPutString("\n");
 	CTOS_PrinterPutString("\n");
 	CTOS_PrinterPutString("\n");
        return TRUE;      
}