/**
  * @file circarray.c
  * @brief  A simple circular array / message buffer implementation
  * @author Eason Smith
  * @date 4/26/2016
  *
  */


#include "circarray.h"

/**
 * @fn void buf_clearbytes(CircArr_InitTypeDef* arr)
 * @brief function to clear (zero) all values from the circular array
 * @param arr a pointer to a circular array object
 * @return
 */
void buf_clear(CircArr_InitTypeDef* arr)
{
for(uint32_t i = 0; i < arr->size; ++i)
	arr->buf[i] = 0;
}

/**
 * @fn void buf_delete(CircArr_InitTypeDef* arr)
 * @brief function to deallocate the internal array used within the struct. Disables the CircArray. resets  size and index data
 * @param arr a pointer to a circular array object
 * @return returns true if successful, false if the array did not exist
 */
bool buf_delete(CircArr_InitTypeDef* arr){
	 if (!arr){
		 //check for unitialized circ array struct. return false because we don't want to accidentally delete it.
		 return false;
	 }

   buf_clear(arr); //zero out internal array data
   free(arr->buf); //delete the internal dyn allocated array

   //reset all values
   arr->n_r =0;
   arr->n_w = 0;
   arr->size = 0;
   arr->buf = NULL;
   arr->enabled = false;

   //array was deleted successfully
   return true;
}



/**
 * @fn void initCircArray(CircArr_InitTypeDef* arr)
 * @brief function to initialize a circular array
 * @param arr a pointer to a circular array object
 * @param size the size of the circular array to be initialized
 * @return
 */
void initCircArray(CircArr_InitTypeDef* arr, uint32_t size)
{

	//check for already initialized circarray. make resize another function
   if (arr->enabled){
	   return;
	}

   arr->size = size;
   arr->n_r = 0;
   arr->n_w = 0;

   arr = malloc(size*sizeof(uint8_t));

   buf_clear(arr);

}

/**
 * @fn void buf_resize(CircArr_InitTypeDef* arr, uint32_t newsize)
 * @brief function to dynamically resize an existing circarray (keeping the data)
 * @param arr a pointer to a circular array object
 * @param newsize the new array size to allocate for the circular array
 * @return
 */
bool buf_resize(CircArr_InitTypeDef* arr, uint32_t newsize)
{
	if(!arr)
		{ return false; }

	//try to reallocate size
	arr->buf = (uint8_t*)realloc(arr->buf , newsize*sizeof(uint8_t));

	if (arr->buf)
	{
	//it worked. update circarray struct members
	arr->n_r = (arr->n_r % arr->size);
	arr->n_w = (arr->n_w % arr->size);
	arr->size = newsize;
	}
	else
	{
		//didnt work. disable the array. call buf_delete?
		printf("mem realloc error in circarray.c resize function\n");
		arr->enabled = false;
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
 * @brief function to return number of bytes available to read in the circular array buffer
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
if ( buf_full(arr) ) { return (0); } //or just increment n_r and n_w?

arr->buf[arr->n_w++ % arr->size] = c;
return (1);
}
