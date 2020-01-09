/*������ʾ��
*��˷��0809��INT3�����Ƚ�0832��0~5v
*0809Ƭѡ��ַ0x2A0��0832Ƭѡ��ַ
*/
#define port_ADC0809 0x2A0
#define port_DAC0832 0x280
#include<fstream>
#include<iostream>
using namespace std;
struct M_ADC0809 {
	BYTE *Data_Point;//������ָ��
	int port_change;
	M_ADC0809():port_change(port_ADC0809+3) { //��IN3��ת��Ϊ��
		Data_Point = (BYTE *)malloc(6000);		/*����ռ����ڴ��¼������*/
		if(!Data_Point) {
			printf("No memory!\7");
			exit(0);
		}
	}
	void Record() {
		BYTE data;
		printf("Press any key to record!\n");	/*¼����ʾ*/
		getch();
		printf("¼���С���\n");	/*¼����ʾ*/
		fstream file;
		file.open("SoundOut.txt",ios::out);
		for(int i=0; i<30000; i++) {
			/*����A/D,�ɼ�6000�����ݷ��ڿ��ٵ��ڴ�ռ���*/
			PortWriteByte(port_change,0);//��ɫ��������
			//delay();
			PortReadByte(port_change,&data);//ʡ�Լ��EOC���ȴ�1ms���ֱ�Ӷ�ȡת�������ֵ
			*(Data_Point+i) = data;
			file<<data<<endl;
		}
		file.close();
		printf("¼���ѽ�����\n");	/*¼����ʾ*/
	}
	void Play() {
		printf("Press any key to playing!\n");	/*������ʾ*/
		getch();
		BYTE data;
		fstream file;
		file.open("SoundOut.txt",ios::in);
		for(int i=0; i<30000; i++) {
			/*��ii�е�6000����D/A���*/
			data = *(Data_Point+i);
			PortWriteByte(port_DAC0832,data);
		}
		file.close();
		printf("Playing end!\n");
	}
	void delay(){
		byte d;
			do{
				PortReadByte(0x28a,&d);
			}while(d&1!=0);
			do{
				PortReadByte(0x28a,&d);
			}while(d&1!=1);
	}
};

#define port_8259 0x2A0
struct M_8259 {
	int port_odd;
	int port_even;
	M_8259():port_odd(port_8259+1),port_even(port_8259) {
		Init();
		InterruptMask_Open();
		Wait_Interrupt();
	}
	void InterruptMask_Open() {
		//��д��OCW1���ж�
		PortWriteByte(port_odd,0x00);
	}
	void Interrupt_End() {
		//��д��OCW2Ϊ00100000�������ղŷ�������ж�
		PortWriteByte(port_even,0x20);
		Wait_Interrupt();
	}
	void IR0(){
		cout<<"�ж�0"<<endl;
		
		Interrupt_End();
	}
	void IR1(){
		cout<<"�ж�1"<<endl;
		
		Interrupt_End();
	}

	void Init() {
		PortWriteByte(port_even,0x13);//ICW1
		PortWriteByte(port_odd,0xB0);//ICW2
		PortWriteByte(port_odd,0x03);//ICW4,�Զ������ն˷�ʽ 
	}
	int Read_ISR() {
		byte data;
		PortWriteByte(port_even,0x0B);
		PortReadByte(port_even,&data);
		return data;
	}
	void Wait_Interrupt(){
		cout<<"Wait  Interrupt...."<<endl;
		while(Read_ICheck()==-1){
			Sleep(20);
		}

	}
	int Read_IRR() {
		byte data;
		PortWriteByte(port_even,0x0A);
		PortReadByte(port_even,&data);
		return data;
	}
	int Read_ICheck() {
		//��ȡ�жϲ�ѯ�֣����λ��ʾ�����жϣ�����λ��ʾ����һ���ж�
		byte data;
		PortWriteByte(port_even,0x0C);
		PortReadByte(port_even,&data);
		//cout<<(int)data<<endl;
		if((int)data/128==1){
			switch((int)data%8){
			case 0:IR0();break;
			case 1:IR1();
			default:;
			}
			return (int)data%8;
		}
		else return -1;
	}
	int Read_IMR() {
		byte data;
		PortReadByte(port_odd,&data);
		return data;
	}
};

struct M_rgy {
	int port_h;
	int port_r;
	int Sleep_time;
	
	M_rgy():port_h(0x2B8),port_r(0x2B0),Sleep_time(10) {

	}
	void Print_Kong() {
		byte Lie[8]= {0x04,0xef,0x55,0x22,0x1c,0x08,0x08,0x3e};
		byte h=0x01;
		for(int i=0; i<8; ++i) {
			PortWriteByte(port_r,Lie[i]);
			PortWriteByte(port_h,h);
			h=h<<1;
			Sleep(Sleep_time);
		}
	}
	void Print_Null() {
		PortWriteByte(port_h,0x00);
	}
};

#define port_8255 0x288
#define port_74LS273 0x290
struct M_8255 {
	int port;
	M_8255():port(port_8255) {
		PortWriteByte(port+3,0x81);
	}
};
#include"time.h"
struct LCD:public M_8255 {
	int port_data;
	int port_ctr;
	int Addr_start;
	int Youbiao_isON;
	LCD():M_8255(),port_data(port_74LS273),port_ctr(port_8255+1),Addr_start(128) ,Youbiao_isON(0){};

	void cmd_setup() {
//PC1��0,pc0��0 ��LCD I��=0��W�ˣ�0��
		PortWriteByte(port_ctr,0x00);
//PC2��1 ��LCD E�ˣ�1��
		PortWriteByte(port_ctr,0x04);
//PC2��0,��LCD E�ˣ�0��
		PortWriteByte(port_ctr,0x01);
	}

	void Clear_All() {
		PortWriteByte(port_data,0x01);
		cmd_setup();
	}

	void data_setup() {
//PC1��0,pc0��0 ��LCD I��=0��W�ˣ�0��
		PortWriteByte(port_ctr,0x01);
//PC2��1 ��LCD E�ˣ�1��
		PortWriteByte(port_ctr,0x05);
//PC2��0,��LCD E�ˣ�0��
		PortWriteByte(port_ctr,0x01);
	}

	void Display_Line(int n,int data[8]) {
		switch(n) {
			case 1:
				Addr_start=128;
				break;
			case 2:
				Addr_start=144;
				break;
			case 3:
				Addr_start=136;
				break;
			default:
				Addr_start=152;
		}
		for(int i=0; i<8; i++) {
			PortWriteByte(port_data,Addr_start);
			cmd_setup();
			PortWriteByte(port_data,data[i]>>8);
			data_setup();
			PortWriteByte(port_data,data[i]%256);
			data_setup();
			Addr_start++;
		}
	}
	void Display_Update_Car_Empty() {
		struct tm *local;
		time_t t=time(NULL);
		local=localtime(&t);
		cout<<"Date: "<<local->tm_mon<<" / "<<local->tm_mday<<endl;
		cout<<"Time: "<<local->tm_hour<<" : "<<local->tm_min<<endl;

		int B_mon,B_mday,B_hour,B_min;
		B_mon=0x3030+((local->tm_mon+1)/10)*256+(local->tm_mon+1)%10;
		B_mday=0x3030+(local->tm_mday/10)*256+local->tm_mday%10;
		B_hour=0x3030+(local->tm_hour/10)*256+local->tm_hour%10;
		B_min=0x3030+(local->tm_min/10)*256+local->tm_min%10;
		Display_Update_ch(2,3,B_mon);
		Display_Update_ch(2,5,B_mday);
		Display_Update_ch(3,3,B_hour);
		Display_Update_ch(3,5,B_min);
	}
	void Display_Update_ch(int n,int r,int data) {
		switch(n) {
			case 1:
				Addr_start=128;
				break;
			case 2:
				Addr_start=144;
				break;
			case 3:
				Addr_start=136;
				break;
			default:
				Addr_start=152;
		}
		PortWriteByte(port_data,Addr_start+r);
		cmd_setup();
		PortWriteByte(port_data,data>>8);
		data_setup();
		PortWriteByte(port_data,data%256);
		data_setup();
	}

	void Display_Youbiao_ON(){
		//��ʾ�α�
		PortWriteByte(port_data,0x0F);
			cmd_setup();
	}
	void Display_Youbiao_OFF(){
		PortWriteByte(port_data,0x0C);
			cmd_setup();
	}
	void Display_Self_Check() {
//
//�����Լ죬���Ժ�
//
//
		Clear_All();
		int Data_Self_Check[8]= {0xBFAA,0xBBFA,0xD7D4,0xBCEC,0xA3AC,0xC7EB,0xC9D4,0xBAF2};
		Display_Line(2,Data_Self_Check);
	}
	void Display_Wait_ICcard() {
//
//__ __ ״̬���� __ __
//__ __ �����IC �� __
//
		Clear_All();
		int Data_Condition_OK[8]= {0xA1A0,0xA1A0,0xD7B4,0xCCAC,0xD5FD,0xB3A3,0xA1A0,0xA1A0};
		int Data_Wait_ICcard[8]= {0xA1A0,0xA1A0,0xC7EB,0xB2E5,0xC8EB,0x4944,0xBFA8,0xA1A0};
		Display_Line(2,Data_Condition_OK);
		Display_Line(3,Data_Wait_ICcard);
	}
	void Display_Setup() {
//__ __ __ �� �� __ __ __
//�� �� �� :_ __ 07 .0 __
//�� �� �� �� :_ 1. 20 __
//ҹ �� �� �� :_ 1. 40 __
		int Data_Setup[8]= {0xA1A0,0xA1A0,0xA1A0,0xC9E8,0xD6C3,0xA1A0,0xA1A0,0xA1A0};
		int Data_Starting_Fare[8]= {0xC6F0,0xB2BD,0xBCDB,0x3A00,0xA1A0,0x3037,0x2E30,0xA1A0};
		int Data_PerPrice_Day[8]= {0xB0D7,0xCCEC,0xB5A5,0xBCDB,0x3A00,0x312E,0x3230,0xA1A0};
		int Data_PerPrice_Night[8]= {0xD2B9,0xBCE4,0xB5A5,0xBCDB,0x3A00,0x312E,0x3430,0xA1A0};
		Display_Line(1,Data_Setup);
		Display_Line(2,Data_Starting_Fare);
		Display_Line(3,Data_PerPrice_Day);
		Display_Line(4,Data_PerPrice_Night);
	}
	void Display_Car_Empty() {
//__ __ __ �� ӭ __ __ __
//�� �� :_ 12 /_ 09 __ __
//ʱ �� :_ 15 :_ 30 __ __
//
		int Data_Welcome[8]= {0xA1A0,0xA1A0,0xA1A0,0xBBB6,0xD3AD,0xA1A0,0xA1A0,0xA1A0};
		int Data_Date[8]= {0xC8D5,0xC6DA,0x3A00,0x3132,0x2F00,0x3039,0xA1A0,0xA1A0};
		int Data_Time[8]= {0xCAB1,0xBCE4,0x3A00,0x3135,0x3A00,0x3330,0xA1A0,0xA1A0};
		int Blank[8]= {0xA1A0,0xA1A0,0xA1A0,0xA1A0,0xA1A0,0xA1A0,0xA1A0,0xA1A0};
		Display_Line(1,Data_Welcome);
		Display_Line(2,Data_Date);
		Display_Line(3,Data_Time);
		Display_Line(4,Blank);

		Display_Update_Car_Empty();
	}
	void Display_Update_Car_Full(double Mileage_Thistime,double Price_Thistime) {
		int Meter_0,Meter_1;
		Meter_0=0x2E30+(int)(Mileage_Thistime*10)%10;
		Meter_1=0x3030+(int)Mileage_Thistime/10*256+((int)Mileage_Thistime*10%100/10);
		Display_Update_ch(3,6,Meter_0);
		Display_Update_ch(3,5,Meter_1);
		
		int Money_0,Money_1;
		Money_0=0x2E30+(int)(Price_Thistime*10)%10;
		Money_1=0x3030+(int)Price_Thistime/10*256+((int)Price_Thistime*10%100/10);
		Display_Update_ch(4,6,Money_0);
		Display_Update_ch(4,5,Money_1);
	}
	void Display_Car_Full(double Per_Price) {
//���� :_ 1. 20 __ __ Ԫ   �磺7.0
//ʱ�� :_ 00 :_ 23 __ __  �磺1:23   //��ʻʱ����ʱȥ��������ʵ���걸ϵͳ������Ϊ�����Ƽ�
//�ۼ� �� �� :_ __ ._ __
//�ۼ� �� �� :_ __ ._ __
		int Data_Per_Price[8]= {0xB5A5,0xBCDB,0x3A00,0x312E,0x3230,0xA1A0,0xA1A0,0xD4AA};
		//int Data_Total_Time[8]= {0xCAB1,0xBCE4,0x3A00,0x3030,0x3A00,0x3031,0xA1A0,0xA1A0};
		int Data_Total_Meter[8]= {0xC0DB,0xBCC6,0xC0EF,0xB3CC,0x3A00,0x3030,0x2E30,0xA1A0};
		int Data_Total_Money[8]= {0xC0DB,0xBCC6,0xBDF0,0xB6EE,0x3A00,0x3037,0x2E30,0xA1A0};
		Display_Line(1,Data_Per_Price);
		//Display_Line(2,Data_Total_Time);
		Display_Line(3,Data_Total_Meter);
		Display_Line(4,Data_Total_Money);
		int Per_p_1=0x302E+(int)Per_Price*256;
		int Per_p_0=0x302E+(int)10*(Per_Price-(int)Per_Price);
		Display_Update_ch(1,3,Per_p_1);
		Display_Update_ch(1,4,Per_p_0);
	}
};


/**����Ӳ������˵��
*8255 ��ַ��Ϊ288
*8255�����ڷ�ʽ0��A������B�������C�ڸ�4λ�������������4λ����������
*
*�������߽�c�ڸ�λ��������߽�c�ڵ�λ����
*/
#define port_8255_C 0x28A
struct KeyBoard {
	/**���̹�������
	*1. �����߶�����ʱ����������Ӧ��ȫΪ1
	*2. �����ַ�1��˵�����а����������ˣ���Ϊ��������ȥ�������һ�г��ַ�1
	*3. �������͸��У�����ȥ�������һ�з�1����c�У���һ�п���ֱ�Ӽ���õ�
	**/
	int port_c;
	int result;
	int row;
	int col;
	int ret;//���ذ�����ֵ
	byte condition;

	KeyBoard():port_c(port_8255+2) {

		init();
		cout<<"keyboard started."<<endl;
	}
	void init() {
		PortWriteByte(port_c+1,0x81);//�����֣�1 00 0 0 00 1  C�ڸ�4λ�������4λ����
	}

	bool IsPRES() {
//���������У���������Ƿ�Ϊ1������˵���а������£�תȥread,��*�����������䣩
		PortWriteByte(port_c,0x00);//����������
		PortReadByte(port_c,&condition);
		if((int)condition%16==15)return false;
		else {
//������һ����������������ʵ�飬
//û����һ������һ�ΰ����У����ʶ��
			Sleep(50);
			PortReadByte(port_c,&condition);
			if((int)condition%16==15) {
				return false;
//isPRES();
			}
			//change();
			//cout<<"PRES!"<<endl;
			//read();
			return true;
		}
	}
	void read() {
		int a=15-condition%15;
		col=0;
		while(a!=0) {
			a/=2;
			col++;
		}
//�������ͣ���ȡ��1ʱ�ĸ��У�ͬʱȥ��������һ�б�������
		byte line=0xEF;
		row=0;
		do {
			PortWriteByte(port_c,line);
			PortReadByte(port_c,&condition);
			++row;
			line=line<<1;
		} while((int)condition%16==15);

		if(col==1) {
			if(row==1)ret=1;
			else if(row==2)ret=2;
			else if(row==3)ret=3;
		} else if(col==2) {
			if(row==1)ret=4;
			else if(row==2)ret=5;
			else if(row==3)ret=6;
		}
		cout<<"the board is "<<row<<" "<<col<<", ret is "<<ret<<endl;
	}

	int Pressed_Button() {
		//���������Ϊ��ѯʽ��һ���ӿں�����ʼ�յȴ��������£�һ�����·������õļ�ֵ
		while(!IsPRES());
		read();
		cout<<"Button "<<ret<<" is Pressed."<<endl;
		return ret;
	}
};

#define port_8254 0x298
struct M_8254 {
	int port_ctr;
	int result;
	M_8254():port_ctr(port_8254+3) {
		Init_Gate0();
		Init_Gate2();
		result=0;
	}
	void Init_Gate0() { //ͨ��0���˶μ�����ֵ�ɹ�֮���жϵ���,��֮��������伶��������
		PortWriteByte(port_ctr,0x30);//�����֣�00 11 000 0ͨ��0�������Ƽ���
		PortWriteByte(port_ctr-3,0x00);
		PortWriteByte(port_ctr-3,0x00);
		cout<<"8254 init, Gate 0 chosed"<<endl;
	}
	void Init_Gate1() { //ͨ��1���˶�Ӳ��ʱ����һ��ˢ����Ļ��Ƶ��
		PortWriteByte(port_ctr,0x77);//�����֣�01 11 011 1ͨ��1��10���Ƽ���
		PortWriteByte(port_ctr-2,0x00);
		PortWriteByte(port_ctr-2,0x00);
		cout<<"8254 init, Gate 1 chosed"<<endl;
	}
	void Init_Gate2() { //ͨ��2�����ڸ������������ӡ����δ��ȫ��
		PortWriteByte(port_ctr,0xB7);//�����֣�10 11 011 1ͨ��2����ʽ3��BCD����
		PortWriteByte(port_ctr-1,0xe9);
		PortWriteByte(port_ctr-1,0xf1);
		cout<<"8254 init, Gate 2 chosed"<<endl;
		PortWriteByte(0x289,0x00);//Ĭ��Ϊ��������
	}

	void Buzzer_Call() {
//8255 B7����Gate2��out���ӷ��������������Ϊ8254�����ڷ�ʽ3ʱ��GateΪ0Ĭ��Ϊ�����������ߣ�
		cout<<"Buzzer_Call init"<<endl;
		for(int i=0; i<8; ++i) {
			PortWriteByte(0x289,0x80);
			Sleep(150);//��Ӧ����ʱ��
			cout<<"end:"<<endl;
			PortWriteByte(0x289,0x00);
			Sleep(30);//��Ӧ���ǳ�Ĭʱ��
		}
		cout<<"Buzzer_Call ended"<<endl;
	}

	int read() {
		PortWriteByte(port_ctr,0x00);//�����֣�00 00 000 0ͨ��0����
		byte L;
		byte H;
		PortReadByte(port_ctr-3,&L);
		PortReadByte(port_ctr-3,&H);
		result=65536-(256*H+L);
		cout<<"8254������ֵΪ��"<<result<<endl;
		if(result>60000)Init_Gate0();
		return result;
	}
};

#define port_DAC0832 0x280

struct M_DAC0832 {
	int port_ctr;
	int speed;

	M_DAC0832():port_ctr(port_DAC0832),speed(0) {
		PortWriteByte(port_ctr,speed);
		cout<<"M_0832 init, Speed_0 = 0"<<endl;
	}
	void Speed_Update(int sp) {
		PortWriteByte(port_ctr,sp);
		cout<<"Speed update, Speed_now = "<<speed<<endl;
	}

};

struct TAXI:public M_DAC0832,M_8254 {
//TAXI ��̳�M_DAC0832оƬ,M_8254оƬ,�ڴ�����������������
//�̳�speed��Ӽ��٣�ͬʱ����������ݳ�Աstate״̬
//state:0-δ������1-���У�10-��ֹ�ȴ��������У�����0��
	int state;
	double Meter_Total;
	double Meter_Thistime;
	double temp;
	double factor;
	TAXI():M_DAC0832(),state(0),Meter_Thistime(0),Meter_Total(0),temp(0),factor(1440) {};
	double Read_Meter_Thistime() {
		int Now=this->read();
		//8254�����ǵ�������������װ����һ������ֵʱ����ֽϴ����
		//�ڳ��⳵�ٶȲ���ʱû�����⣬���ٶȴﵽ�嵵ʱ�кܴ���
		if(Now>=temp) {
			Meter_Thistime+=(double)(Now-temp)/factor;
			temp=Now;
		} else {
			Meter_Thistime+=(double)(Now)/factor;
			temp=Now;
		}
		return Meter_Thistime;
	}
	double Read_Meter_Total() {
		return Meter_Total+Meter_Thistime;
	}
	void Thistime_Start() {
		Meter_Thistime=0;
		temp=0;
	}
	int Thistime_End() {
		Meter_Total+=Read_Meter_Thistime();
		cout<<"�������н����������Ϊ��"<<Meter_Thistime<<endl;
		return Meter_Thistime;
	}
	void Start_or_SpeedUp() {
		if(state==0) {
			state=1;//��������
			speed=90;
			this->Speed_Update(speed);
			return;
		}
		if(speed<245)speed+=10;
		else speed=254;
		this->Speed_Update(speed);

		if(state==10) {
			state=1;
			cout<<"Taxi continue runing"<<endl;
		}
	}
	void Stop_or_SpeedDown() {
		if(state==10) {
			state=0;
			cout<<"Taxi stop"<<endl;
			return;
		}
		if(speed>70)speed-=10;
		else speed=61;
		Speed_Update(speed);
		if(speed==61) {
			state=10;
			cout<<"Taxi wait"<<endl;
		}
	}
};

struct TAXIMETER {
	int state;
	/**state ����Ƽ�����״̬
	*0 IC��δ��
	*10 �س�״̬
	*11 �ճ�״̬
	*12 �Լ�״̬
	*13 ����״̬
	*14 �Ƽ���ͣ״̬
	**/
	double Per_Price_Day;
	double Per_Price_Night;
	double Per_Price_Now;
	
	double Starting_Price;
	int Total_Times;//�ۻ��ؿʹ���
	int Total_Price;
//����Ӫ���е�һЩ����
	int Money_Thistime;
	int Time_Start;
	int Time_End;

	LCD lcd;
	M_8255 m_8255;
	M_8254 m_8254;
	KeyBoard kb;
	TAXI taxi;
	M_rgy rgy;

	TAXIMETER():state(0) {
		History_Read();
		//Wait_IC();
	};
	~TAXIMETER() {
//���������п��Լӡ�дӲ�̡�
	}
	void History_Read() {
		//�˲�����ʹ�ô洢�����жϵ�洢
		Per_Price_Day=1.2;
		Per_Price_Night=1.4;
		taxi.Meter_Total=123;
		Total_Price=374;
		Total_Times=12;
		Starting_Price=7;
		cout<<"Read Log OK, "<<Per_Price_Day<<" "<<Per_Price_Night<<" "
		    <<taxi.Meter_Total<<" "<<Total_Price<<" "<<Total_Times<<" "<<endl;
	}

	void History_Write() {

	}
	double Caculate_Money_Thistime(double meter) {
		double M;
		if(meter<3)M=7;
		else{
		M=Per_Price_Now*(meter-3)+7;
		M*=10;
		M=(int)(M+0.5);
		M=(double)M/10;//��ȷ��С�����һλ
		}
		cout<<"Meter:  "<<meter<<",Money:  "<<M<<endl;
		return M;
	}
	void Journey_Start() {
		state=10;
		struct tm *local;
		time_t t=time(NULL);
		local=localtime(&t);
		if(local->tm_hour>18||local->tm_hour<6)Per_Price_Now=Per_Price_Night;
		else Per_Price_Now=Per_Price_Day;
		rgy.Print_Null();
		taxi.Thistime_Start();
		lcd.Display_Car_Full(Per_Price_Now);
	}
	void Journey_End() {
		state=11;
		rgy.Print_Kong();
		taxi.Thistime_End();
		m_8254.Buzzer_Call();
		Sleep(1000);
		lcd.Display_Car_Empty();
	}
	void State_Change(int ret) {
		//��κ������ݼ��̲���ִ��״̬֮�����ת���룬�������������ĵ�ת��
		switch(state) {
			case 11: {
				//�ճ�
				if(ret==6)taxi.Start_or_SpeedUp();
				else if(ret==3)taxi.Stop_or_SpeedDown();
				else if(ret==1) {
					Journey_Start();
				} else if(ret==2) {
					state=13;
					lcd.Display_Setup();
				} else return;
				break;
			}
			case 10: {
				//�س�״̬�������ԼӼ���
				if(ret==6)taxi.Start_or_SpeedUp();
				else if(ret==3)taxi.Stop_or_SpeedDown();
				else if(ret==1) {
					Journey_End();
				}
				return;
			}
			case 13: {
				//����״̬�����������û��߼Ӽ���
				if(ret==6)taxi.Start_or_SpeedUp();
				else if(ret==3)taxi.Stop_or_SpeedDown();
				else if(ret==2){
					if(lcd.Youbiao_isON==0){
						lcd.Display_Youbiao_ON();
						lcd.Youbiao_isON=1;
					}
					else if(lcd.Youbiao_isON==1){
						lcd.Display_Youbiao_OFF();
						lcd.Youbiao_isON=2;
					}
					else if(lcd.Youbiao_isON==2){
						lcd.Display_Car_Empty();
						lcd.Youbiao_isON=0;
						state=11;
					}
					}
				//else if(ret==5);
				else return;
			}
			default:
				;
		}
	}
	void Wait_IC() {
		lcd.Display_Wait_ICcard();
//��������ѯ�ȴ����̲���IC������ģ�⣩
		while(kb.Pressed_Button()!=4);
		state=12;
		Self_Check();
	}
	void Self_Check() {
		lcd.Display_Self_Check();
		Sleep(1500);
		state=11;
		lcd.Display_Car_Empty();
		Update();
//�ȴ��ճ��Ʊ����£����ǰ���������
	}
	void Update() {
		while(1) {
			if(kb.IsPRES()) {
				kb.read();
				State_Change(kb.ret);
			} else {
				switch(state) {
					case 11:
						lcd.Display_Update_Car_Empty();
						rgy.Print_Kong();
						break;
					case 13://�˴���Ϊ���ã����Լ�һЩ���׵Ĳ���
						rgy.Print_Kong();
						break;
					case 10:
						lcd.Display_Update_Car_Full(taxi.Read_Meter_Thistime(),Caculate_Money_Thistime(taxi.Meter_Thistime));
						break;
					default:;
				}
			}
		}
	}
};
