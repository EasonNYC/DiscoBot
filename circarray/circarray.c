/**
  * @file circarray.c
  * @brief  A simple circular array / message buffer implementation
  * @author Eason Smith
  * @date 4/26/2016
  *
  */
#include "circarray.h"



/**
 * @fn void initCircArray(CircArr_InitTypeDef* arr)
 * @brief function to initialize a circular array
 * @param arr a pointer to a circular array object
 * @return
 */
void initCircArray(CircArr_InitTypeDef* arr)
{
	arr->n_r = 0;
	arr->n_w = 0;
   arr->size = CIRCSIZE;
	//clear out the array upon initialization
	for(uint32_t i =0; i < CIRCSIZE;++i)
	{
		arr->buf[i] = 0;
	}
}



/**
 * @fn uint8_t buf_empty(CircArr_InitTypeDef* arr)
 * @brief returns whether the circular array is empty
 * @param a pointer to a circular array object
 * @return
 */
uint8_t buf_empty(CircArr_InitTypeDef* arr)
{
	return (arr->n_r == arr->n_w);
}

/**@ fn uint8_t buf_full(CircArr_InitTypeDef* arr)
 * @brief buf_full returns whether the circular array is full or not
 * @param arr a pointer to a circular array object
 * @return
 */
uint8_t buf_full(CircArr_InitTypeDef* arr)
{
	return (((arr->n_r % arr->size) - (arr->n_w % arr->size)) == 1);
}

/**@fn uint8_t buf_available(CircArr_InitTypeDef* arr)
 * @brief function to return number of bytes available in the circular array buffer
 * @param arr a pointer to a circular array object
 * @return
 */
uint8_t buf_available(CircArr_InitTypeDef* arr)
{
	return (arr->n_w - arr->n_r);
}

/**
 * @fn uint8_t buf_getbyte(CircArr_InitTypeDef* arr)
 * @brief buf getbyte returns the next byte available in the circular array
 * @param arr a pointer to a circular array object
 * @return
 */
uint8_t buf_getbyte(CircArr_InitTypeDef* arr)
{
if ( buf_empty(arr) ) {
	return (0); }

uint8_t c = arr->buf[arr->n_r++  % arr->size];
return c;
}

/**@fn int8_t buf_putbyte(CircArr_InitTypeDef* arr, uint8_t c)
 * @brief buf_putbyte accepts a single byte c and puts it in the circular array.
 * @param arr a pointer to a circular array object
 * @param c an unsigned byte to be added to the array
 * @return
 */
uint8_t buf_putbyte(CircArr_InitTypeDef* arr, uint8_t c)
{
if ( buf_full(arr) ) { return (0); }

arr->buf[arr->n_w++ % arr->size] = c;
return (1);
}
