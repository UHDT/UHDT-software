XBee 64-bit destination address is currently DEFINED in xbee.h.
Using another XBee for the ground station will require that
this address be changed.  I opted to define these values
instead of passing them as parameters to the Tx functions
for convenience.


Current iteration of Rx code will store a packet in the
Rx buffer, then echo that packet if the packet was a Rx
packet (API ID = 0x90).  Edit the "else" section of this
function if you want to do anything else once a new
packet is received (make sure to check the API ID first).


USART_init, Rx interrupt, and SendByte functions are
all configured for USART1 (pins 6 and 7).