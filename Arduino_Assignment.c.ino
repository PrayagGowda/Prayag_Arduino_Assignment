#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <util/delay.h>

void USART_Init(unsigned int ubrr)
{
   UBRR0H = (unsigned char)(ubrr>>8);
   UBRR0L = (unsigned char)ubrr;
   UCSR0B = (1<<RXEN0)|(1<<TXEN0);
   UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit(char b)
{
  while(!(UCSR0A & (1<<UDRE0)));
  UDR0 = b;
}
 
unsigned char USART_Received(void)
{
  while(!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

void USART_Flush()
{
  unsigned char dummy;
  while(UCSR0A & (1<<RXC0)) dummy = UDR0;
}

void Led_Blink()
{
  PORTB ^= (1 << 5);  
  _delay_ms(1000);
}

void clk_dc()
{
  char b[] = "  Forward Direction...";
  char f[] = "Enter S - Stop";

  for(int i = 0; b[i] != '\0'; i++) 
  { 
    USART_Transmit(b[i]); 
  }
  USART_Flush(); USART_Transmit('\r'); USART_Transmit('\n');
  USART_Flush(); USART_Transmit('\r'); USART_Transmit('\n');

  for(int i = 0; f[i] != '\0'; i++) 
  { 
    USART_Transmit(f[i]); 
  }
  USART_Flush(); USART_Transmit('\r'); USART_Transmit('\n');
  USART_Flush(); USART_Transmit('\r'); USART_Transmit('\n');

  PORTD = (1 << 5); 
}

void anti_dc()
{
  char c[] = "   Reverse Direction....";

  for(int i = 0; c[i] != '\0'; i++) 
  { 
    USART_Transmit(c[i]); 
  }
  USART_Flush(); USART_Transmit('\r'); USART_Transmit('\n');
  USART_Flush(); USART_Transmit('\r'); USART_Transmit('\n');

  PORTD = (1 << 6); 
}

void stop_dc()
{
  char d[] = "   Stop...!";

  for(int i = 0; d[i] != '\0'; i++) 
  { 
    USART_Transmit(d[i]); 
  }
  USART_Flush(); USART_Transmit('\r'); USART_Transmit('\n');
  USART_Flush(); USART_Transmit('\r'); USART_Transmit('\n');

  PORTD &= ~(1 << 5); 
  PORTD &= ~(1 << 6);  
}

int main()
{
  DDRB = (1 << 5);   
  DDRD = (1 << 5) | (1 << 6); 

  USART_Init(MYUBRR);
 
  char a[] = "Enter F for Forward, R for Reverse, S for Stop:";
  char z[] = "Led Blinking for One Second.......!";

  for(int i = 0; z[i] != '\0'; i++) 
  { 
    USART_Transmit(z[i]);
  }
  USART_Flush(); USART_Transmit('\r'); USART_Transmit('\n');
  USART_Flush(); USART_Transmit('\r'); USART_Transmit('\n');

  for(int i = 0; a[i] != '\0'; i++) 
  { 
    USART_Transmit(a[i]);
  }
  USART_Flush(); USART_Transmit('\r'); USART_Transmit('\n');
  USART_Flush(); USART_Transmit('\r'); USART_Transmit('\n');

  while (1) 
  {
      Led_Blink();  

      if (UCSR0A & (1 << RXC0)) 
      {  
          char K = USART_Received();
          USART_Transmit(K);

          if (K == 'F') clk_dc();
          if (K == 'R') anti_dc();
          if (K == 'S') stop_dc();
      }
  }
}
