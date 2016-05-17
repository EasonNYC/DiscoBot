/**
  * @file circarray.h
  * @brief  A simple circular array / message buffer implementation
  * @author Eason Smith
  * @date 4/26/2016
  *
  */
#ifndef CIRCARRAY_CIRCARRAY_H_
#define CIRCARRAY_CIRCARRAY_H_

#include <stdio.h>

#define CIRCSIZE 200

typedef enum _bool {FALSE = 0, false = 0, TRUE = 1, true = 1} bool;

/**
 * @struct _CircArr_InitTypeDef
 * @brief a struct representing a circular array object
 */
typedef struct _CircArr_InitTypeDef
{
	bool enabled;   //bool of whether this circarray struct is enabled and currently usable

	uint32_t size;		// the array size in bytes to be created for the circular array (add resize capability?)
	uint32_t n_r;     //read index upcounter (if n_r % size, will give current read position)
	uint32_t n_w;    //write index upcounter (if n_w % size, will give current write position)

	uint8_t* buf;    //a ptr to a circular array (malloc not great for embedded but maybe worth it for portability)

} volatile CircArr_InitTypeDef;



//public methods
void initCircArray(CircArr_InitTypeDef* arr, uint32_t size);
uint8_t buf_empty(CircArr_InitTypeDef* arr);
uint8_t buf_full(CircArr_InitTypeDef* arr);
uint8_t buf_available(CircArr_InitTypeDef* arr);
uint8_t buf_getbyte(CircArr_InitTypeDef* arr);
uint8_t buf_putbyte(CircArr_InitTypeDef* arr, uint8_t c);
void buf_test(CircArr_InitTypeDef* arr);
uint8_t buf_peek_byte(CircArr_InitTypeDef* arr);


#endif /* CIRCARRAY_CIRCARRAY_H_ */

/************example usage
#include "circarray/circarray.h"


CircArr_InitTypeDef msg; //first create 1 circular array buffer called msg

int main(void)
{

  //initialize the array before using
  initCircArray(&msg);

  printf("buffer is empty? ( %i ) Adding 3 bytes.\n",buf_empty(&msg));

  //add 3 bytes to the array
  buf_putbyte(&msg,12);
  buf_putbyte(&msg,13);
  buf_putbyte(&msg,14);

  printf("buffer is empty? (%i) \n",buf_empty(&msg));
  printf("the buffer has (%i) bytes available to be read\n", buf_available(&msg));

  //read next byte from array
  uint8_t tempbyte = buf_getbyte(&msg);
  printf("got 1 byte \n");

  printf("the buffer now has (%i) bytes available. the byte was %i\n", buf_available(&msg), tempbyte);
  printf("getting two more bytes \n");
  uint8_t tempbyte2 = buf_getbyte(&msg);
  uint8_t tempbyte3 = buf_getbyte(&msg);
  printf("the buffer now has (%i) bytes available. the byte values were %i %i \n", buf_available(&msg), tempbyte2, tempbyte3);
  printf("buffer is empty? (%i) \n",buf_empty(&msg));

  //asking for more data when there is none returns 0
  printf("getting a byte when empty just returns (%i) \n",buf_getbyte(&msg));

  printf("filling the array up till its full. \n");
  for (uint8_t i = 0;i < msg.size; ++i)
	  buf_putbyte(&msg,4);

 //prove it's full
  printf("done. Full? (%i). array contains %i of %i elements. removing one element.\n", buf_full(&msg),buf_available(&msg), msg->size);
  tempbyte = buf_getbyte(&msg);

  //prove no longer full
  printf("Full? (%i)\n", buf_full(&msg));


  printf("Test done!\n");

 */
