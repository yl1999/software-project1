#include "reg51.h" 
typedef unsigned char uchar;
typedef unsigned int uint;
xdata uchar wx _at_ 0x8002;   
xdata uchar dx _at_ 0x8004;
void delay(uint z);			
void write_data(uint n);	
void flicker();			
void beep1();
void show();
void init();
uint date[7]={0,0,0,0,0,0,0};	
uint n=0;		
uint second1=0,minute1=0,hour1=0,i=0,flag=0,j=0;

uchar TIMECODE[11]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00};		
					
uchar beepTimes[24][3]=
 {{6,50,0},{7,0,0},{7,25,0},{8,20,0},{8,30,0},{9,15,0},{9,25,0},{10,10,0},{10,30,0},{11,15,0},{11,25,0},{12,10,0},{13,20,0},{13,30,0},{14,15,0},{14,25,0},{15,10,0},{16,30,0},{17,0,0},{17,50,0},{18,0,0},{21,30,0},{0,0,0},{0,0,0}};
sbit beep=P1^1;
sbit led=P1^0;
sbit guangbo=P1^2;
sbit nao_h=P1^3;
sbit nao_m=P1^4;
sbit guang_h=P1^5;
sbit guang_m=P1^6;
sbit set=P1^7;
code uchar select[] = { 0x40,0x20,0x10,0x08,0x04,0x02,0x01 };
void main()
{

	init();
	while(1)
	{
		if(flag==0)
		{
			flicker(); beep1();
			if(nao_m==0)
			{
				 minute1++;
				if(minute1==60)
				minute1=0;
			 flicker();
			 	while(!nao_m);
			}
		   if(nao_h==0)
		   {
		   	hour1++;
		   	if(hour1==24)hour1=0;
			 flicker();
			 while(!nao_h);
			 }
		}
		if(set==0)
		{
			delay(50);		
			if(set==0)
			{
			flag=1;
			while(!set);			
			show();	
			}						
		}
	}
}
void init()
{
	TMOD=0x02;	
	TH0=0x56;	
	TL0=0x56;	
	TR0=1;		
	EA=1;		
	ET0=1;			
	beep=0;
	  led=0;
	  guangbo=0;
	  nao_h=1;
	  nao_m=1;
	  guang_h=1;
	  guang_m=1;
	  set=1;
	  flag=0;
}
void beep1()
{	for(i=0;i<23;i++)
		 {
		 if(hour1==beepTimes[i][0]&&minute1==beepTimes[i][1]&&((second1==0)||(second1==1)||(second1==2)	  )	)
		 {led=1; beep=1;
		  }
		  else{ beep=0;led=0;  }
		  }
		if(hour1==beepTimes[23][0]&&minute1==beepTimes[23][1])
			guangbo=1;
		else guangbo=0;
}
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=114;y>0;y--)
		;
}
void  write_data(uint n)
{
	
	dx=TIMECODE[n];
	
}

void flicker()
{	
	wx=select[1];
	write_data(date[1]);
	delay(1);
	dx=~0xff;		  

	wx=select[2];
	write_data(date[2]);
	delay(1);
	dx=~0xff;	

	wx=select[3];
	write_data(date[3]);	
	delay(1);
	dx=~0xff;

	wx=select[4];
	write_data(date[4]);	
	delay(1);
	dx=~0xff;

	wx=select[5];
	write_data(date[5]);
	delay(1);
	dx=~0xff;
		
	wx=select[6];
	write_data(date[6]);
	delay(1);
	dx=~0xff;
}

void serve()interrupt 1
{
	 
	 n++;										        
	 if(n==3000)		
	 {	 
	     n=0;					                        
	     second1++;
		 if(second1==60)
		 {
		     second1=0;
		     minute1++;
			 if(minute1==60)
			 {
			     minute1=0;
				 hour1++;
				 if(hour1==24)
				 {	    
				    hour1=0;
				 }
			 }
		 }
		 date[6]=second1%10;
		 date[5]=second1/10;
		 date[4]=minute1%10;
		 date[3]=minute1/10;
		 date[2]=hour1%10;
		 date[1]=hour1/10;	
	 }

}
void show()
{  while(1)
{
	if(nao_h==0)
		{
			delay(50);
			if(nao_h==0)
				beepTimes[22][0]=beepTimes[22][0]+1;
				if(beepTimes[22][0]==24)beepTimes[22][0]=0;
			while(!nao_h);			
			wx=select[1];			
			write_data(beepTimes[22][0]/10);	
			delay(100);	 
			wx=select[2];
			write_data(beepTimes[22][0]%10);
			delay(1);
		}
		if(nao_m==0)
		{
			delay(50);
			if(nao_m==0)
				beepTimes[22][1]=beepTimes[22][1]+1;
			while(!nao_m);
			if(beepTimes[22][1]==60)beepTimes[22][1]=0;
				wx=select[3];
			write_data(beepTimes[22][1]/10);
			delay(100);
				wx=select[4];
			write_data(beepTimes[22][1]%10);
		}
		if(guang_h==0)
		{
			delay(50);
			if(guang_h==0)
				beepTimes[23][0]=beepTimes[23][0]+1;
			while(!guang_h);
			if(beepTimes[23][0]==24)beepTimes[23][0]=0;
				wx=select[1];
			write_data(beepTimes[23][0]/10);
			delay(100); 
				wx=select[2];
			write_data(beepTimes[23][0]%10);
			delay(1);
		}
		if(guang_m==0)
		{
			delay(50);
			if(guang_m==0)
				beepTimes[23][1]=beepTimes[23][1]+1;
			while(!guang_m);
			if(beepTimes[23][1]==60)beepTimes[23][1]=0;
				wx=select[3];
			write_data(beepTimes[23][1]/10);
			delay(100);
			
				wx=select[4];
			write_data(beepTimes[23][1]%10);;
			}
		if(set==0)
		{
		delay(50);
		if(set==0)
			flag=0;
		while(!set);
		break;
		}
	}	   
}