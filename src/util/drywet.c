#include "util/mem.h"

#include "bypass/bypass.h"

void bypass_perform(float *in, float *out, int n){
	while (n--) *out++ = *in;
}