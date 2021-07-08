#include"C8051F020.h"
int floor = 0;
unsigned int up_request[5]= {0,0,0,0,0};
unsigned int floor_request[5]= {0,0,0,0,0};
unsigned int down_request[5]= {0,0,0,0,0};
//unsigned int leds[2]= {0,0};
int i=0;
sbit redled = P2^3;
sbit greenled = P0^7;
int doorstate = 0;
sbit OpenDoor=P2^7;
// inside_buutons 
sbit floor0 = P0^2;
sbit floor1 = P0^3;
sbit floor2 = P0^4;
sbit floor3 = P0^5;
sbit floor4 = P0^6;
sbit ir1 = P2^2;
sbit ir2 = P2^5 ;
int enter = 0 ;
int exit = 0 ;
sbit overload = P1^7;
int persons = 0 ; 
sbit motor1 = P2^0; 
sbit motor2 = P2^1; 
sbit floor0_down = P2^4;
sbit floor2_down = P2^6;
void timer0_init(void)
{   CKCON &= 0xF0; //T0M = 0; Timer 0 uses SysClock/12 
    TMOD = (TMOD & 0xF0) | 0x01; /* Set T/C0, Mode mode1(tm1=0&tm0=1)*/ 
    TH0 = 0xFFFF;
    TL0 = 0x63BF ; //240ms
    ET0 = 1; 
}
void motor_delay()
{
    int i = 0;
    int j = 0;
    for (i=0;i<1000;++i){
    for(j=0;j<100;++j){
 }}}
void delay(){
    int i = 0;
    int j = 0;
    for (i=0;i<500;++i){
    for(j=0;j<100;++j){
}}}
void door_delay()
{
    int i = 0;
    int j = 0;
    for (i=0;i<1000;++i){
    for(j=0;j<1000;++j){
        if ((ir1 == 0) && (ir2 ==1))
        { if(exit== 0){enter=1;}
        if(exit==1){persons-- ;exit = 0; delay();}}
        if ((ir1 == 1) && (ir2 ==0))
        { if(enter == 0){
        exit=1;
    }
    if(enter==1){
        persons++;
        enter = 0; 
    delay();
}}}}}
void motor_delay1()
{
    int i = 0;
    int j = 0;
    for (i=0;i<1000;++i){
    for(j=0;j<90;++j){
    }}}
void up()
{
    motor1 = 1 ;
    motor2 = 0;
    motor_delay();
    motor1 = 0 ;
    motor2=1;
    motor_delay1();
    motor2=0;
}
void down()
    { motor1 = 0 ; 
    motor2 = 1 ;
    motor_delay();
    motor2 = 0 ;
    motor1=1 ;
    motor_delay1();
    motor1=0;
}
void open_door(){
    redled = 1;
    greenled = 0;
    door_delay();
    if (persons > 4 ){overload = 1; open_door(); }
    if (persons< 5) {overload = 0 ;} 
    redled =0;
    greenled=1;
}
void requests(void){
    //inside _buttons
    if(!(floor0)){
    floor_request[0]=1; 
    }
    if(!(floor1)){
    floor_request[1]=1;
    }
    if(!(floor2)){
    floor_request[2]=1;
    }
    if(!(floor3)){ floor_request[3]=1;
    }
    if(!(floor4)){
    floor_request[4]=1; 
    }
    if(!(floor0_down)){ 
    down_request[0]=1;
    }
    if(!(floor2_down)){
    down_request[2]=1;
    }
    if(!(P5&0x01)) { 
    up_request[1]=1 
    } 
    if(!(P5&0x02)) { //P5,"1"
    up_request[2]=1;
    } 
    if(!(P5&0x04)) { //"P5,"2"
    up_request[3]=1;
    } 
    if(!(P5&0x08)) { //P5,"3"
    up_request[4]=1;
    } 
    if((!OpenDoor && !doorstate)) { 
    open_door();
 }}
//executed every 65536 0+count
void timer0_ISR() interrupt 1 {
    TF0 = 0 ; requests();
}
void seg_dispaly(floor){
    if(floor==0) P1 = 0x3F;
    else if(floor==1) P1 = 0x06;
    else if(floor==2) P1 = 0x5B;
    else if(floor==3) P1 = 0x4F;
    else if(floor==4) P1 = 0x66;
}
void main(void){ 
// diable the watch dog
    WDTCN = 0x0DE;
    WDTCN = 0x0AD;
    OSCICN = 0x014; // 2MH clock
    // config cross bar
    XBR0 = 0x00;
    XBR1 = 0x00;
    XBR2 = 0x040; // Cross bar enabled , weak Pull-up enabled
    P0MDOUT |= 0x80; 
    P1MDOUT |= 0x7F;
    P2MDOUT |= 0x00;
    timer0_init();
    EA=1;
    TR0 = 1; 
    overload=0;
    redled=0;
    greenled=1;
    requests();
    while(1)  {
    seg_dispaly(floor); 
    i=0;
    for(i=0;i<5;i++){
    if(( floor_request[i] || up_request[i] || down_request[i])&& i>floor ) 
    {
    while(floor != i )
        { doorstate=1;
        up();
        floor++;
        seg_dispaly(floor);
    if( floor_request[floor] || up_request[floor]|| down_request[floor] )
    { 
        floor_request[floor]=0;
        up_request[floor]=0;
        down_request[floor]=0;
        doorstate = 0;
        open_door(); 
    }}}}
    i=4;
    for( i=4;i>=0;i--){
    if(( floor_request[i] || down_request[i] || up_request[i]) && i< floor ){
    while(floor != i )
    {   doorstate=1;
        down();
        floor--;
        seg_dispaly(floor);
        if( floor_request[floor] || down_request[floor]||up_request[floor])
        {  floor_request[floor]=0;
        down_request[floor]=0; 
        up_request[floor]=0;
        doorstate=0;
        open_door();
 }}}}}}