/*
Copyright (c) 2012-2019 Ben Croston

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* See the following for up to date information:
 * https://www.raspberrypi.org/documentation/hardware/raspberrypi/revision-codes/README.md (obsolete 2021-08-09)
 * https://www.raspberrypi.com/documentation/computers/raspberry-pi.html#raspberry-pi-revision-codes
 */
// 2021-10-30 Zero 2 W
// 2022-11-29 BCM2711
// 2023-08-29 update links, revision

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "cpuinfo.h"

unsigned get_revision(void) {
  FILE *fp;
  uint32_t n = 0;

  if ((fp = fopen("/proc/device-tree/system/linux,revision", "r"))) {
    if (fread(&n, sizeof(n), 1, fp) != 1) {
      fclose(fp);
      return 0;
    }
  }
  fclose(fp);
  return ntohl(n);
}

int get_rpi_info(rpi_info *info)
{
   FILE *fp;
   char buffer[1024];
   char hardware[32];
   char revision[32];
   int found = 0;
   int len;

   if ((fp = fopen("/proc/device-tree/system/linux,revision", "r"))) {
      uint32_t n;
      if (fread(&n, sizeof(n), 1, fp) != 1) {
         fclose(fp);
         return -1;
      }
      sprintf(revision, "%x", ntohl(n));
      found = 1;
   }
   else if ((fp = fopen("/proc/cpuinfo", "r"))) { // Pre Device Tree
      while(!feof(fp) && fgets(buffer, sizeof(buffer), fp)) {
         sscanf(buffer, "Hardware	: %s", hardware);
         if (strcmp(hardware, "BCM2708") == 0 ||
             strcmp(hardware, "BCM2709") == 0 ||
             strcmp(hardware, "BCM2711") == 0 ||
             strcmp(hardware, "BCM2835") == 0 ||
             strcmp(hardware, "BCM2836") == 0 ||
             strcmp(hardware, "BCM2837") == 0 ) {
            found = 1;
         }
         sscanf(buffer, "Revision	: %s", revision);
      }
   }
   else
      return -1;
   fclose(fp);

   if (!found)
      return -1;

   if ((len = strlen(revision)) == 0)
      return -1;

   if (len >= 6 && strtol((char[]){revision[len-6],0}, NULL, 16) & 8) {
      // new scheme
      //info->rev = revision[len-1]-'0';
      strcpy(info->revision, revision);
      switch (revision[len-3]) {
         case '0' :
            switch (revision[len-2]) {
               case '0': info->type = "Model A"; info->p1_revision = 2; break;
               case '1': info->type = "Model B"; info->p1_revision = 2; break;
               case '2': info->type = "Model A+"; info->p1_revision = 3; break;
               case '3': info->type = "Model B+"; info->p1_revision = 3; break;
               case '4': info->type = "Pi 2 Model B"; info->p1_revision = 3; break;
               case '5': info->type = "Alpha"; info->p1_revision = 3; break;
               case '6': info->type = "Compute Module 1"; info->p1_revision = 0; break;
               case '8': info->type = "Pi 3 Model B"; info->p1_revision = 3; break;
               case '9': info->type = "Zero"; info->p1_revision = 3; break;
               case 'a': info->type = "Compute Module 3"; info->p1_revision = 0; break;
               case 'c': info->type = "Zero W"; info->p1_revision = 3; break;
               case 'd': info->type = "Pi 3 Model B+"; info->p1_revision = 3; break;
               case 'e': info->type = "Pi 3 Model A+"; info->p1_revision = 3; break;
               default : info->type = "Unknown"; info->p1_revision = 3; break;
            } break;
         case '1':
            switch (revision[len-2]) {
               case '0': info->type = "Compute Module 3+"; info->p1_revision = 0; break;
               case '1': info->type = "Pi 4 Model B"; info->p1_revision = 3; break;
               case '2': info->type = "Zero 2 W"; info->p1_revision = 3; break;
               case '3': info->type = "Pi 400"; info->p1_revision = 3; break;
               case '4': info->type = "Compute Module 4"; info->p1_revision = 3; break;
               default : info->type = "Unknown"; info->p1_revision = 3; break;
            } break;
         default: info->type = "Unknown"; info->p1_revision = 3; break;
      }

      switch (revision[len-4]) {
         case '0': info->processor = "BCM2835"; break;
         case '1': info->processor = "BCM2836"; break;
         case '2': info->processor = "BCM2837"; break;
         case '3': info->processor = "BCM2711"; break;
         default : info->processor = "Unknown"; break;
      }
      switch (revision[len-5]) {
         case '0': info->manufacturer = "Sony"; break;
         case '1': info->manufacturer = "Egoman"; break;
         case '2': info->manufacturer = "Embest"; break;
         case '3': info->manufacturer = "Sony Japan"; break;
         case '4': info->manufacturer = "Embest"; break;
         case '5': info->manufacturer = "Stadium"; break;
         default : info->manufacturer = "Unknown"; break;
      }
      switch (strtol((char[]){revision[len-6],0}, NULL, 16) & 7) {
         case 0: info->ram = "256M"; break;
         case 1: info->ram = "512M"; break;
         case 2: info->ram = "1G"; break;
         case 3: info->ram = "2G"; break;
         case 4: info->ram = "4G"; break;
         case 5: info->ram = "8G"; break;
         default: info->ram = "Unknown"; break;
      }
   } else {
      // old scheme
      info->ram = "Unknown";
      info->manufacturer = "Unknown";
      info->processor = "Unknown";
      info->type = "Unknown";
      strcpy(info->revision, revision);

      uint32_t rev;
      sscanf(revision, "%x", &rev);
      rev = rev & 0xefffffff;       // ignore preceeding 1000 for overvolt

      if (rev == 0x0002 || rev == 0x0003) {
         info->type = "Model B";
         info->p1_revision = 1;
         info->ram = "256M";
         info->manufacturer = "Egoman";
         info->processor = "BCM2835";
      } else if (rev == 0x0004) {
         info->type = "Model B";
         info->p1_revision = 2;
         info->ram = "256M";
         info->manufacturer = "Sony UK";
         info->processor = "BCM2835";
      } else if (rev == 0x0005) {
         info->type = "Model B";
         info->p1_revision = 2;
         info->ram = "256M";
         info->manufacturer = "Qisda";
         info->processor = "BCM2835";
      } else if (rev == 0x0006) {
         info->type = "Model B";
         info->p1_revision = 2;
         info->ram = "256M";
         info->manufacturer = "Egoman";
         info->processor = "BCM2835";
      } else if (rev == 0x0007) {
         info->type = "Model A";
         info->p1_revision = 2;
         info->ram = "256M";
         info->manufacturer = "Egoman";
         info->processor = "BCM2835";
      } else if (rev == 0x0008) {
         info->type = "Model A";
         info->p1_revision = 2;
         info->ram = "256M";
         info->manufacturer = "Sony UK";
         info->processor = "BCM2835";
      } else if (rev == 0x0009) {
         info->type = "Model A";
         info->p1_revision = 2;
         info->ram = "256M";
         info->manufacturer = "Qisda";
         info->processor = "BCM2835";
      } else if (rev == 0x000d) {
         info->type = "Model B";
         info->p1_revision = 2;
         info->ram = "512M";
         info->manufacturer = "Egoman";
         info->processor = "BCM2835";
      } else if (rev == 0x000e) {
         info->type = "Model B";
         info->p1_revision = 2;
         info->ram = "512M";
         info->manufacturer = "Sony UK";
         info->processor = "BCM2835";
      } else if (rev == 0x000f) {
         info->type = "Model B";
         info->p1_revision = 2;
         info->ram = "512M";
         info->manufacturer = "Qisda";
         info->processor = "BCM2835";
      } else if (rev == 0x0010) {
         info->type = "Model B+";
         info->p1_revision = 3;
         info->ram = "512M";
         info->manufacturer = "Sony UK";
         info->processor = "BCM2835";
      } else if (rev == 0x0011) {
         info->type = "Compute Module 1";
         info->p1_revision = 0;
         info->ram = "512M";
         info->manufacturer = "Sony UK";
         info->processor = "BCM2835";
      } else if (rev == 0x0012) {
         info->type = "Model A+";
         info->p1_revision = 3;
         info->ram = "256M";
         info->manufacturer = "Sony UK";
         info->processor = "BCM2835";
      } else if (rev == 0x0013) {
         info->type = "Model B+";
         info->p1_revision = 3;
         info->ram = "512M";
         info->manufacturer = "Embest";
         info->processor = "BCM2835";
      } else if (rev == 0x0014) {
         info->type = "Compute Module 1";
         info->p1_revision = 0;
         info->ram = "512M";
         info->manufacturer = "Embest";
         info->processor = "BCM2835";
      } else if (rev == 0x0015) {
         info->type = "Model A+";
         info->p1_revision = 3;
         info->ram = "Unknown";
         info->manufacturer = "Embest";
         info->processor = "BCM2835";
      } else {  // don't know - assume revision 3 p1 connector
         info->p1_revision = 3;
      }
   }
   return 0;
}

// Return rpi_info as a python compatible dictionary string
// This is easier than trying to read rpi_info in python!
char *get_rpi_dict() {
  rpi_info info;
  char dict_info[127];

  get_rpi_info(&info);
  sprintf(dict_info,
          "{'p1_revision':%d, 'type':'%s', 'ram':'%s', 'processor':'%s', "
          "'manufacturer':'%s', 'revision':'%.32s'}",
          info.p1_revision, info.type, info.ram, info.processor,
          info.manufacturer, info.revision);
  return strdup(dict_info);
}

void free_memory(char* ptr) {
    free(ptr);
}

/*
Correct as at 2023-08-27
32 bits
NEW           23: will be 1 for the new scheme, 0 for the old scheme
MEMSIZE       20: 0=256M 1=512M 2=1G 3=2GB 4=4GB 5=8GB
MANUFACTURER  16: 0=Sony UK 1=Egoman 2=Embest 3=Sony Japan 4=Embest 5=Stadium
PROCESSOR     12: 0: BCM2835 1: BCM2836 2: BCM2837 3: BCM2711
TYPE          04: 0:A 1:B 2:A+ 3:B+ 4:2B 5:Alpha 6:CM1 8:3B 9:Zero a:CM3 c:Zero W
                  d:3B+ e:3A+ 10:CM3+ 11:4B 12:Zero 2 W 13:400 14:CM4 15:CM4S
REV           00: 0=REV0 1=REV1 2=REV2

pi2 = 1<<23 | 2<<20 | 1<<12 | 4<<4 = 0xa01040

--------------------
SRRR MMMM PPPP TTTT TTTT VVVV

S scheme (0=old, 1=new)
R RAM 
M manufacturer
P processor
T type
V revision (0-15)

*/
