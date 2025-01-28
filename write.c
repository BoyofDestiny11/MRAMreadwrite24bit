#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "spi23x1024.c"

#define ADDRESS_WIDTH 24    //or 17??
#define MAX_MEM_SPEED 40000000

uint16_t img_write2() {int size = (int)pow(2, ADDRESS_WIDTH - 3);
  uint8_t csv_data[size];

   FILE *file =fopen("Aubie.csv", "r");

   if (file == NULL) {
      perror("Error opening file");
      return 1;
   }

    // Discard the first line (header)
   char buffer[16];
   if (fgets(buffer, sizeof(buffer), file) == NULL) {
      perror("Error reading header");
      fclose(file);
      return 1;
   }

   for (int i = 0; i < ((int)pow(2, ADDRESS_WIDTH - 3)); i++) {
      if (fgets(buffer, sizeof(buffer), file) == NULL) {
         perror("Error reading data");
         fclose(file);
         return 1;
      }
        
        // Parse the "Address" and "Byte" data
      unsigned int address;
      unsigned int byte;
      if (sscanf(buffer, "%4x,%2x", &address, &byte) != 2) {
         perror("Error parsing data");
         fclose(file);
         return 1;
      }
   
        // Store the byte data in the array
      csv_data[i] = (uint8_t)byte;
   }

   fclose(file);


   spi23x1024_init(MAX_MEM_SPEED);
   uint16_t address_idx;
   for (address_idx = 0; address_idx <= SPI_MEM_MAX_ADDRESS; address_idx++) {
   	//printf("%x\n", address_idx);
      uint8_t next_value = csv_data[address_idx];
      spi23x1024_write_byte(address_idx, next_value);
   }	
   spi23x1024_close();
   printf("Done Writing Image to Chip 1.\n");


   spi23x1024_init2(MAX_MEM_SPEED);
   for (address_idx = 0; address_idx <= SPI_MEM_MAX_ADDRESS; address_idx++) {
   	//printf("%x\n", address_idx);
      uint8_t next_value = csv_data[address_idx];
      spi23x1024_write_byte(address_idx, next_value);
   }	
   spi23x1024_close2();
   printf("Done Writing Image to Chip 2.\n");

   return 0;
}

int main()
{
   if (img_write2() != 0)  {
      printf("Error: image not written.");
   }
}
