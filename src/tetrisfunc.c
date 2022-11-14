#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdbool.h> /* bool */
/* Declare a helper function which is local to this file */
static void num32asc( char * s, int );
uint8_t pixelArray[128*4] = {0};
extern const uint8_t const font[128*8];
extern char textbuffer[4][16];
#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)
#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)
#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)
#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)
/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}
uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

void display_init(void) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}
void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	
	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}
void display_image(void) {
	int i, j;
	
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22); 
		spi_send_recv(i);	
		spi_send_recv(0x0);
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
			for(j = 0; j < 128; j++){
				spi_send_recv(pixelArray[i*128+j]); //skickar en column av pixlar till nuvarande page
													
				pixelArray[i*128+j]=0x0; 
			}
	}
	
	
}
void drawPixel(int x, int y) {
	pixelArray[y/8 * 128 + x] |= (1 << (y-(y/8)*8));	
}
//ritar en siffra på position (x,y)
void printNumber(int x, int y, int number) {
	switch(number) {
		case 0:
			drawPixel(x,y);
			drawPixel(x,y+1);
			drawPixel(x,y+2);
			drawPixel(x-1,y);
			drawPixel(x-2,y);
			drawPixel(x-3,y);
			drawPixel(x-4,y);
			drawPixel(x-4,y+1);
			drawPixel(x-4,y+2);
			drawPixel(x-1,y+2);
			drawPixel(x-2,y+2);
			drawPixel(x-3,y+2);
			drawPixel(x-4,y+2);
		break;
		case 1:
			drawPixel(x-1,y);
			drawPixel(x-4,y);
			drawPixel(x,y+1);
			drawPixel(x-1,y+1);
			drawPixel(x-2,y+1);
			drawPixel(x-3,y+1);
			drawPixel(x-4,y+1);
			drawPixel(x-4,y+2);
		break;
		case 2:
			drawPixel(x,y);
			drawPixel(x,y+1);
			drawPixel(x,y+2);
			drawPixel(x-1,y+2);
			drawPixel(x-2,y+2);
			drawPixel(x-2,y+1);
			drawPixel(x-2,y);
			drawPixel(x-3,y);
			drawPixel(x-4,y);
			drawPixel(x-4,y+1);
			drawPixel(x-4,y+2);
		break;
		case 3:
			drawPixel(x,y);
			drawPixel(x,y+1);
			drawPixel(x,y+2);
			drawPixel(x-1,y+2);
			drawPixel(x-2,y+2);
			drawPixel(x-2,y+1);
			drawPixel(x-2,y);
			drawPixel(x-3,y+2);
			drawPixel(x-4,y+2);
			drawPixel(x-4,y+1);
			drawPixel(x-4,y);
		break;
		case 4:
			drawPixel(x,y);
			drawPixel(x,y+2);
			drawPixel(x-1,y+2);
			drawPixel(x-2,y+2);
			drawPixel(x-2,y+1);
			drawPixel(x-2,y);
			drawPixel(x-3,y+2);
			drawPixel(x-4,y+2);
			drawPixel(x-1,y);
		break;
		
		case 5:
			drawPixel(x,y);
			drawPixel(x,y+1);
			drawPixel(x,y+2);
			drawPixel(x-2,y+2);
			drawPixel(x-2,y+1);
			drawPixel(x-2,y);
			drawPixel(x-3,y+2);
			drawPixel(x-4,y+2);
			drawPixel(x-4,y+1);
			drawPixel(x-4,y);
			drawPixel(x-1,y);
		break;
		
		case 6:
			drawPixel(x,y);
			drawPixel(x,y+1);
			drawPixel(x,y+2);
			drawPixel(x-2,y+2);
			drawPixel(x-2,y+1);
			drawPixel(x-2,y);
			drawPixel(x-3,y+2);
			drawPixel(x-4,y+2);
			drawPixel(x-4,y+1);
			drawPixel(x-4,y);
			drawPixel(x-1,y);
			drawPixel(x-3,y);
		break;
		
		case 7:
			drawPixel(x,y);
			drawPixel(x,y+1);
			drawPixel(x,y+2);
			drawPixel(x-1,y+2);
			drawPixel(x-2,y+2);
			drawPixel(x-3,y+2);
			drawPixel(x-4,y+2);
		break;
		
		case 8:
			drawPixel(x,y);
			drawPixel(x,y+1);
			drawPixel(x,y+2);
			drawPixel(x-1,y+2);
			drawPixel(x-2,y+2);
			drawPixel(x-2,y+1);
			drawPixel(x-2,y);
			drawPixel(x-3,y+2);
			drawPixel(x-4,y+2);
			drawPixel(x-4,y+1);
			drawPixel(x-4,y);
			drawPixel(x-1,y);
			drawPixel(x-3,y);
		break;
		
		case 9:
			drawPixel(x,y);
			drawPixel(x,y+1);
			drawPixel(x,y+2);
			drawPixel(x-1,y+2);
			drawPixel(x-2,y+2);
			drawPixel(x-2,y+1);
			drawPixel(x-2,y);
			drawPixel(x-3,y+2);
			drawPixel(x-4,y+2);
			drawPixel(x-1,y);
		break;
		
	}
}


void printScore(int n, int score) {
	int i;
	int j;
	for (i=n;i>0;i--) {
		int x = 100; //the x värde där poängen ska stå
		int y = 4 + (i-1)*5; //y värdet beror på vilken siffra som skrivs
		printNumber(x, y, score%10); //modulo 10 ger sista siffran
		score /= 10; //dela med 10 för att få nästa siffra
	}
}
void drawSpeed (int n, int speed) { 
	int i;
	int j;
	for (i=n;i>0;i--) {
		int x = 117; 
		int y = 4 + (i-1)*5; 
		printNumber(x, y, speed%10); 
		speed /= 10;	
	}
}
//rita spelets kanter
void drawBorder(void) {
	int i;
	int j;
	for (i=0;i<128;i++) {
		pixelArray[0/8 * 128 + i] |= (1 << (0-(0/8)*8));
		pixelArray[31/8 * 128 + i] |= (1 << (31-(31/8)*8));
	}
	for (i=0;i<32;i++) {
		pixelArray[i/8 * 128 + 0] |= (1 << (i-(i/8)*8));
		pixelArray[i/8 * 128 + 61] |= (1 << (i-(i/8)*8));
		pixelArray[i/8 * 128 + 82] |= (1 << (i-(i/8)*8));
		pixelArray[i/8 * 128 + 127] |= (1 << (i-(i/8)*8));
	}
}
//rita speltexten
void drawGameText(void) {
	//NEXT
	
	//N
	drawPixel(78,9);
	drawPixel(77,9);
	drawPixel(76,9);
	drawPixel(75,9);
	drawPixel(74,9);
	drawPixel(78,10);
	drawPixel(78,11);
	drawPixel(77,11);
	drawPixel(76,11);
	drawPixel(75,11);
	drawPixel(74,11);
	
	//E
	drawPixel(78,13);
	drawPixel(78,14);
	drawPixel(78,15);
	drawPixel(77,13);
	drawPixel(76,13);
	drawPixel(75,13);
	drawPixel(74,13);
	drawPixel(76,13);
	drawPixel(76,14);
	drawPixel(76,15);
	drawPixel(74,13);
	drawPixel(74,14);
	drawPixel(74,15);
	
	//X
	drawPixel(78,17);
	drawPixel(77,17);
	drawPixel(76,18);
	drawPixel(75,17);
	drawPixel(74,17);
	drawPixel(78,19);
	drawPixel(77,19);
	drawPixel(75,19);
	drawPixel(74,19);
	
	//T
	drawPixel(78,21);
	drawPixel(78,22);
	drawPixel(78,23);
	drawPixel(77,22);
	drawPixel(76,22);
	drawPixel(75,22);
	drawPixel(74,22);
	
	
	//SPEED
	
	//S
	drawPixel(123,4);
	drawPixel(123,5);
	drawPixel(123,6);
	drawPixel(122,4);
	drawPixel(121,4);
	drawPixel(121,5);
	drawPixel(121,6);
	drawPixel(120,6);
	drawPixel(119,6);
	drawPixel(119,5);
	drawPixel(119,4);
	
	//P
	drawPixel(123,8);
	drawPixel(122,8);
	drawPixel(121,8);
	drawPixel(120,8);
	drawPixel(119,8);
	drawPixel(123,9);
	drawPixel(123,10);
	drawPixel(122,10);
	drawPixel(121,10);
	drawPixel(121,9);
	
	//E
	drawPixel(123,12); 	
	drawPixel(123,13);
	drawPixel(123,14);
	drawPixel(122,12);
	drawPixel(121,12);
	drawPixel(120,12);
	drawPixel(119,12);
	drawPixel(121,12);
	drawPixel(121,13);
	drawPixel(121,14);
	drawPixel(119,12);
	drawPixel(119,13);
	drawPixel(119,14);
	
	//E
	drawPixel(123,16); 	
	drawPixel(123,17);
	drawPixel(123,18);
	drawPixel(122,16);
	drawPixel(121,16);
	drawPixel(120,16);
	drawPixel(119,16);
	drawPixel(121,16);
	drawPixel(121,17);
	drawPixel(121,18);
	drawPixel(119,16);
	drawPixel(119,17);
	drawPixel(119,18);
	
	//D
	drawPixel(123,20);
	drawPixel(122,20);
	drawPixel(121,20);
	drawPixel(120,20);
	drawPixel(119,20);
	drawPixel(123,21);
	drawPixel(123,22);
	drawPixel(122,22);
	drawPixel(121,22);
	drawPixel(120,22);
	drawPixel(119,22);
	drawPixel(119,21);


	//:
	drawPixel(122,24);
	drawPixel(120,24);


	//SCORE
	//S
	drawPixel(106,4);
	drawPixel(106,5);
	drawPixel(106,6);
	drawPixel(105,4);
	drawPixel(104,4);
	drawPixel(104,5);
	drawPixel(104,6);
	drawPixel(103,6);
	drawPixel(102,6);
	drawPixel(102,5);
	drawPixel(102,4);
	
	//C
	drawPixel(106,8);
	drawPixel(106,9);
	drawPixel(106,10);
	drawPixel(105,8);
	drawPixel(104,8);
	drawPixel(103,8);
	drawPixel(102,8);
	drawPixel(102,9);
	drawPixel(102,10);
	
	
	//O
	drawPixel(106,12);
	drawPixel(106,13);
	drawPixel(106,14);
	drawPixel(105,12);
	drawPixel(104,12);
	drawPixel(103,12);
	drawPixel(102,12);
	drawPixel(102,13);
	drawPixel(102,14);
	drawPixel(103,14);
	drawPixel(104,14);
	drawPixel(105,14);
	drawPixel(106,14);
	
	
	//R
	drawPixel(106,16);
	drawPixel(105,16);
	drawPixel(104,16);
	drawPixel(103,16);
	drawPixel(102,16);
	drawPixel(106,17);
	drawPixel(106,18);
	drawPixel(106,19);
	drawPixel(105,19);
	drawPixel(104,19);
	drawPixel(104,18);
	drawPixel(104,17);
	drawPixel(103,18);
	drawPixel(102,19);
	
	//E
	drawPixel(106,21);
	drawPixel(105,21);
	drawPixel(104,21);
	drawPixel(103,21);
	drawPixel(102,21);
	drawPixel(106,22);
	drawPixel(106,23);
	drawPixel(104,22);
	drawPixel(104,23);
	drawPixel(102,22);
	drawPixel(102,23);
	
	//:
	drawPixel(105,25);
	drawPixel(103,25);
	

}	

//ritar ett 3x3 pixel block vid en viss position
void drawBlock(int posx, int posy) {
	posx=(3*(posx-1))+1;
	posy=(3*(posy-1))+1;
	int i;
	int j;
	for (i=0;i<3;i++) {
		for (j=0;j<3;j++) {
			pixelArray[(posy+j)/8 * 128 + (posx+i)] |= (1 << ((posy+j)-((posy+j)/8)*8));
		}
	}
	
}


//ger längden av en int
int intWidth(int i)
{
	if (i < 0) i = -i;
	if (i <	10) return 1;
	if (i <	100) return 2;
	if (i <	1000) return 3;
	if (i <	10000) return 4;
	if (i <	100000) return 5;
	if (i < 1000000) return 6;      
	if (i < 10000000) return 7;
	if (i < 100000000) return 8;
	if (i < 1000000000) return 9;
	return 10;
}


void display_update(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}
/* Helper function, local to this file.
   Converts a number to hexadecimal ASCII digits. */
static void num32asc( char * s, int n ) 
{
  int i;
  for( i = 28; i >= 0; i -= 4 )
    *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}
/*
 * itoa
 * 
 * Simple conversion routine
 * Converts binary to decimal numbers
 * Returns pointer to (static) char array
 * 
 * The integer argument is converted to a string
 * of digits representing the integer in decimal format.
 * The integer is considered signed, and a minus-sign
 * precedes the string of digits if the number is
 * negative.
 * 
 * This routine will return a varying number of digits, from
 * one digit (for integers in the range 0 through 9) and up to
 * 10 digits and a leading minus-sign (for the largest negative
 * 32-bit integers).
 * 
 * If the integer has the special value
 * 100000...0 (that's 31 zeros), the number cannot be
 * negated. We check for this, and treat this as a special case.
 * If the integer has any other value, the sign is saved separately.
 * 
 * If the integer is negative, it is then converted to
 * its positive counterpart. We then use the positive
 * absolute value for conversion.
 * 
 * Conversion produces the least-significant digits first,
 * which is the reverse of the order in which we wish to
 * print the digits. We therefore store all digits in a buffer,
 * in ASCII form.
 * 
 * To avoid a separate step for reversing the contents of the buffer,
 * the buffer is initialized with an end-of-string marker at the
 * very end of the buffer. The digits produced by conversion are then
 * stored right-to-left in the buffer: starting with the position
 * immediately before the end-of-string marker and proceeding towards
 * the beginning of the buffer.
 * 
 * For this to work, the buffer size must of course be big enough
 * to hold the decimal representation of the largest possible integer,
 * and the minus sign, and the trailing end-of-string marker.
 * The value 24 for ITOA_BUFSIZ was selected to allow conversion of
 * 64-bit quantities; however, the size of an int on your current compiler
 * may not allow this straight away.
 */
#define ITOA_BUFSIZ ( 24 )
char * itoaconv( int num )
{
  register int i, sign;
  static char itoa_buffer[ ITOA_BUFSIZ ];
  static const char maxneg[] = "-2147483648";
  
  itoa_buffer[ ITOA_BUFSIZ - 1 ] = 0;   /* Insert the end-of-string marker. */
  sign = num;                           /* Save sign. */
  if( num < 0 && num - 1 > 0 )          /* Check for most negative integer */
  {
    for( i = 0; i < sizeof( maxneg ); i += 1 )
    itoa_buffer[ i + 1 ] = maxneg[ i ];
    i = 0;
  }
  else
  {
    if( num < 0 ) num = -num;           /* Make number positive. */
    i = ITOA_BUFSIZ - 2;                /* Location for first ASCII digit. */
    do {
      itoa_buffer[ i ] = num % 10 + '0';/* Insert next digit. */
      num = num / 10;                   /* Remove digit from number. */
      i -= 1;                           /* Move index to next empty position. */
    } while( num > 0 );
    if( sign < 0 )
    {
      itoa_buffer[ i ] = '-';
      i -= 1;
    }
  }
  /* Since the loop always sets the index i to the next empty position,
   * we must add 1 in order to return a pointer to the first occupied position. */
  return( &itoa_buffer[ i + 1 ] );
}