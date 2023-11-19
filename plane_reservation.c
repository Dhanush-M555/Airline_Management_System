#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
struct srm_airline
{ char passport[6];
char name[15];
 int seat_num;
 char email[15];
 struct srm_airline*following;
}
*begin,*stream;
struct srm_airline*dummy;

void main()
{ void reserve(int x),cancel(),display(),savefile();
 int choice;
 begin=stream=NULL;
 int num=1;
 do
 {printf("\n\n\t\t **");
printf("\n\t\t *Welcome to SRM AIRLINE SYSTEM");
printf("\n\n\n\t\t Please Enter your choice from below(1-4):");
printf("\n\n\t\t 1.RESERVATION");
printf("\n\n\t\t 2.CANCEL");
printf("\n\n\t\t 3.DISPLAY RECORDS");
printf("\n\n\t\t 4.EXIT");
printf("\n\n\t\t Feel free to ask us-");
printf("\n\n\t\t Enter your choice:");
scanf("%d",&choice); fflush(stdin);
system("cls");
switch(choice)
{
case 1:
reserve(num);
num++ ;
break;
case 2:
cancel();
 break;
case 3:
 display();
 break;
case 4:
 { savefile();
 break;
 }
 default:
 printf("\n\n\t SORRY INVALID CHOICE");
 printf("\n\n\t PLEASE CHOOSE FROM 1-4");
 printf("\n\n\t Do not forget to choose from 1-4");
 }
getch();
} while (choice !=4);
}
//GOOD LUCK*//
void details()
{
printf("\n\t Greetings!");
gets(stream->passport); fflush(stdin); 
printf("\n\t Enter your passport number:");
gets(stream->passport); fflush(stdin);
printf("\n\t Enter you Name:");
gets(stream->name); fflush(stdin);
printf("\n\t Enter your Email address:");
gets(stream->email); fflush(stdin);
}
//GOOD LUCK*//
void details();
void reserve(int x)
{ stream=begin;
if (begin==NULL)
{
//First User//
 begin=stream=(struct srm_airline*)malloc(sizeof(struct srm_airline));
 details();
 stream->following=NULL;
 printf("\n\t Seat Booking Succesful!");
 printf("\n\t Your seat number is: Seat A-%d",x);
 stream->seat_num=x;
 return;
 }
 else if(x>15)
 {
 printf("\n\n Seats Full");
 return;
}
else
{
//next User
 while(stream->following)
 stream=stream->following;
 stream->following=(struct srm_airline*)malloc(sizeof(struct srm_airline));
 stream=stream->following;
 details();
 stream->following=NULL;
 printf("\n\t Seat Booking Succesful!");
 printf("\n\t Your seat number is: Seat A-%d",x);
 stream->seat_num=x;
 return;
}
}
//GOOD LUCK*//
void savefile()
{ FILE*fpointer=fopen("airline records","w");
if (!fpointer)
{ printf("\n Error in opening file");
return;
}
stream=begin;
while (stream)
{ fprintf(fpointer,"%-6s",stream->passport);
 fprintf(fpointer,"%-15s",stream->name);
 fprintf(fpointer,"%-15s",stream->email);
 stream=stream->following;
}
printf("\n\nt Details have been saved to a file(airline records)");
fclose(fpointer);
}
//GOOD LUCK*//
void display()
{ stream=begin;
while(stream)
{
 printf("\n Passport Number:%-6s",stream->passport);
 printf("\n Name:%-15s",stream->name);
 printf("\n Email address:%-15s",stream->email);
 printf("\n Seat number: A-%d",stream->seat_num);
 printf("\n\n++=============================================================++");
 stream=stream->following;
}
}
//GOOD LUCK*//
void cancel()
{
 stream=begin;
 system("cls");
 char passport[6];
 gets(passport); fflush(stdin);
 printf("Enter Passport number to delete record?:");
 gets(passport); fflush(stdin);
 if (strcmp(begin->passport,passport)==0)
 {
 dummy=begin;
 begin=begin->following;
 free(dummy);
 printf("Booking has been deleted");
 return;
 }
 while(stream->following)
 {
 if(strcmp(stream->following->passport,passport)==0)
 {
 dummy=stream->following;
 stream->following=stream->following->following;
 free(dummy);
 printf("Has been deleted");
 getch();
 return;
}
 stream=stream->following;
}
printf("Passport Number is wrong PLEASE check your passport");
}
//GOOD LUCK*/