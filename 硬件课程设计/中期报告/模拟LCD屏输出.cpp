//����̨���ģ��LCDҺ���������
#include<iostream>
using namespace std;

void UI_Setup(){
	cout<<"**ģ��LCDҺ��������--����**"<<endl;
	cout<<"    ----------------"<<endl;
	cout<<"  ||      ����      ||"<<endl;
	cout<<"  ||�𲽼ۣ�  07.0  ||"<<endl;
	cout<<"  ||���쵥�ۣ�1.20  ||"<<endl;
	cout<<"  ||ҹ�䵥�ۣ�1.40  ||"<<endl;
	cout<<"    ----------------"<<endl;	
}

void UI_Setup_Money(){
	cout<<"**ģ��LCDҺ��������--�ȴ��忨**"<<endl;
	cout<<"    ----------------"<<endl;
	cout<<"  ||                ||"<<endl;
	cout<<"  ||    ״̬����    ||"<<endl;
	cout<<"  ||    �����ID��  ||"<<endl;
	cout<<"  ||                ||"<<endl;
	cout<<"    ----------------"<<endl;	
}

void UI_Setup_Time(){
	cout<<"**ģ��LCDҺ��������--ʱ������**"<<endl;
	cout<<"    ----------------"<<endl;
	cout<<"  ||      ��ӭ      ||"<<endl;
	cout<<"  ||���ڣ�12/ 13    ||"<<endl;
	cout<<"  ||ʱ�䣺16: 33    ||"<<endl;
	cout<<"  ||                ||"<<endl;
	cout<<"    ----------------"<<endl;	
}

void UI_DaiJi(){
	cout<<"**ģ��LCDҺ��������--����**"<<endl;
	cout<<"    ----------------"<<endl;
	cout<<"  ||����: 1.00    Ԫ||"<<endl;
	cout<<"  ||����: 12/ 17    ||"<<endl;
	cout<<"  ||�ۼ����: 07.1  ||"<<endl;
	cout<<"  ||�ۼƽ�12.0  ||"<<endl;
	cout<<"    ----------------"<<endl;
}

void UI_YunXing(){
	cout<<"**ģ��LCDҺ��������--����**"<<endl;
	cout<<"    ----------------"<<endl;
	cout<<"  ||                ||"<<endl;
	cout<<"  ||�����Լ죬���Ժ�||"<<endl;
	cout<<"  ||                ||"<<endl;
	cout<<"  ||                ||"<<endl;
	cout<<"    ----------------"<<endl;
}

void UI_Log(){
	cout<<"**ģ��LCDҺ��������--��־**"<<endl;
	cout<<"    ----------------"<<endl;
	cout<<"  ||����:      11/21||"<<endl;
	cout<<"  ||�ؿʹ���:     06||"<<endl;
	cout<<"  ||�����:     03.8||"<<endl;
	cout<<"  ||�����룺   07.00||"<<endl;
	cout<<"    ----------------"<<endl;	
}

int main(){
	UI_YunXing();
	UI_Setup();
	UI_DaiJi();
	
	UI_Setup_Money();
	UI_Setup_Time();
	system("pause");
	return 0;
} 
