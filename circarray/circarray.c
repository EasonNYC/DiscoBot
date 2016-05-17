/**
  * @file circarray.c
  * @brief  A simple circular array / message buffer implementation
  * @author Eason Smith
  * @date 4/26/2016
  *
  */


#include "circarray.h"
#include <stdlib.h>
/**
 * @fn void buf_clearbytes(CircArr_InitTypeDef* arr)
 * @brief function to clear (zero) all values from the circular array
 * @param arr a pointer to a circular array object
 * @return
 */
void buf_clear(CircArr_InitTypeDef* arr)
{
for(uint8_t i = 0; i < arr->size; ++i){
	arr->buf[i] = 0;
}
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
	//printf("initializing circ array\n");
	//check for already initialized circarray. make resize another function
   if (arr->enabled){
	   printf("error circ array already initialized\n");
	   return;
	}

   arr->size = size;
   arr->n_r = 0;
   arr->n_w = 0;
   arr->buf = calloc(size,sizeof(uint8_t));
   //if buf == null error here
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
		{
		printf("error in buf resize: tried to resize a null circarray struct.resize aborted.");
		return false;
		}

   //try to reallocate size
	uint8_t* result = (uint8_t*)realloc(arr->buf , newsize*sizeof(uint8_t));

	if (!result)
	{
		//didnt work. disable the array?
		printf("executing mem realloc did not find enough space when called by circarray.c resize().\n");
		//arr->enabled = false;
		return false;
	}

	//it worked. update array to point to new memory location. update struct members
	arr->buf = result;
	arr->size = newsize;
	return true;
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
	//if isnt empty but mod of indexes equal (modnr == modnw)
	return ((buf_empty(arr)==0)  && ((arr->n_r % arr->size) == (arr->n_w % arr->size) ) );
	//old return (((arr->n_r % arr->size) - (arr->n_w % arr->size)) == 1);
}

/**@fn uint8_t buf_available(CircArr_InitTypeDef* arr)
 * @brief function to return number of bytes available to read in the circular array buffer
 * @param arr a pointer to a circular array object
 * @return returns num bytes available for reading
 */
uint8_t buf_available(CircArr_InitTypeDef* arr)
{

	return arr->n_w - arr->n_r;

}

/**
 * @fn uint8_t buf_getbyte(CircArr_InitTypeDef* arr)
 * @brief buf getbyte returns the next byte available in the circular array
 * @param arr a pointer to a circular array object
 * @return
 */
uint8_t buf_getbyte(CircArr_InitTypeDef* arr)
{
if ( buf_empty(arr) ) { //if empty don't increment and return arbitrary value for now
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
if ( buf_full(arr) ) {
	return (0); } //or just increment n_r and n_w?

arr->buf[arr->n_w++ % arr->size] = c;
return (1);
}
/*
void buf_test(CircArr_InitTypeDef* arr)
{
	  printf("buffer is empty? ( %i )", buf_empty(arr));
	  printf("bytes avail to read: %i", buf_available(arr));
	  printf("Adding 3 bytes.\n");

	  //add 3 bytes to the array
	  buf_putbyte(arr,12);
	  buf_putbyte(arr,13);
	  buf_putbyte(arr,14);

	  printf("buffer is empty? (%i) \n",buf_empty(arr));
	  printf("the buffer has (%i) bytes available to be read\n", buf_available(arr));


	  //resize check
	  printf("resizing the array to %i bytes\n", arr->size*2);
	  uint8_t resizestatus = buf_resize(arr,arr->size*2);
	  printf("resize worked? (%i)\n", resizestatus);
	  printf("current size reported from array: (%i)\n", arr->size);

	  printf("buffer is empty? (%i) \n",buf_empty(arr));
	  printf("the array contains %i of %i elements to read.\n", buf_available(arr), arr->size);


	  //read next byte from array
	  uint8_t tempbyte = buf_getbyte(arr);
	  printf("got 1 byte \n");

	  printf("the buffer now has (%i) bytes available for reading. the byte was %i\n", buf_available(arr), tempbyte);
	  printf("removing last two bytes \n");
	  uint8_t tempbyte2 = buf_getbyte(arr);
	  uint8_t tempbyte3 = buf_getbyte(arr);
	  printf("the buffer now has (%i) bytes available to read. the byte values just read were %i %i \n", buf_available(arr), tempbyte2, tempbyte3);
	  printf("buffer is empty? (%i) \n",buf_empty(arr));

	  //asking for more data when there is none returns 0
	  printf("getting a byte when empty just returns (%i) char \n",buf_getbyte(arr));

	  //prove size
	  printf("Full? (%i). array contains %i of %i elements to read.\n", buf_full(arr),buf_available(arr), arr->size);

	  printf("filling the array up till its full. \n");
	  for (uint8_t i = 0;i < arr->size; i++) {
		  buf_putbyte(arr,1);
	  }
	 //prove it's full
	  printf("done. Full? (%i). array contains %i of %i elements. removing one element.\n", buf_full(arr),buf_available(arr), arr->size);

	  tempbyte = buf_getbyte(arr);

	  //prove no longer full
	  printf("Full? (%i)\n", buf_full(arr));
	 //report elementsl
	 printf("array contains %i of %i elements.\n", buf_available(arr), arr->size);

	 printf("Test complete.\n");
}
*/
