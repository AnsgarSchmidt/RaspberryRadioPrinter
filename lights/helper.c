#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "helper.h"

void pabort(const char *s){
  perror(s);
  abort();
}
