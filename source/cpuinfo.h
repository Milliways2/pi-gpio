
#ifndef CPUINFO_H
#define CPUINFO_H
typedef struct
{
   int p1_revision;	// P1 Header 0:None, 1:Pi B, 2:Pi B V2, 3:40 pin
   char *ram;
   char *manufacturer;
   char *processor;
   char *type;
   char revision[1024];
} rpi_info;
#endif /* CPUINFO_H */

int get_rpi_info(rpi_info *info);
