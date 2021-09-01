#include "evelopbuf/evelopbuf.h"



#include <stdio.h>
#include <math.h>
#include "util/mem.h"
#include "util/util.h"






evelope *evelope_new(evelope* ep, int type, int length, float amplitude, int attacksamples, int releasesamples){

   switch (type){
      case 0:
         ep = evelope_gen_no_evelope(ep, type, length, amplitude); // only for debugging
         break;
      case 1:
         ep = evelope_gen_parabolic(ep, type, length, amplitude); 
         break;
      case 2:
         ep = evelope_gen_trapezoidal(ep, type, length, amplitude, attacksamples, releasesamples); 
         break;         
      case 3:
         ep = evelope_gen_raised_cosine_bell(ep, type, length, amplitude, attacksamples, releasesamples); 
         break;
      default:
         error("evelope_new: unsupported evelope type: %d, please check again!!!", type);
   }

   return ep;
}


void evelope_free(evelope* ep){
   free(ep->data);
   free(ep);
   post("evelope free!");
}


evelope *evelope_gen_trapezoidal(evelope* ep, int type, int length, float amplitude, int attacksamples, int releasesamples){
   ep = malloc(sizeof(evelope));
   ep->data = malloc(sizeof(float) * length);
   ep->type = type;
   ep->length = length;
   ep->attacksamples = attacksamples;
   ep->releasesamples = releasesamples;

   float amp = 0;
   float amplitudeIncrement = 0; // little bit rudandence but structure would be clear

   amplitudeIncrement = amplitude / (float) attacksamples; 
   for(int i=0;i<attacksamples;i++){
      ep->data[i] = amp;
      amp = amp + amplitudeIncrement;
   }

   amplitudeIncrement = 0;
   for(int i=attacksamples;i<length-releasesamples;i++){
      ep->data[i] = amp;
      amp = amp + amplitudeIncrement;
   }

   amplitudeIncrement = - amplitude / (float) releasesamples;
   for(int i=length-releasesamples;i<length;i++){
      ep->data[i] = amp;
      amp = amp + amplitudeIncrement;
   }

   return ep;
}


evelope *evelope_gen_parabolic(evelope* ep, int type, int length, float amplitude){
   ep = malloc(sizeof(evelope));
   ep->data = malloc(sizeof(float) * length);
   ep->type = type;
   ep->length = length;
   ep->attacksamples = 0;
   ep->releasesamples = 0;

   float amp = 0;
   float rdur = 1.0 / (float) length;
   float rdur2 = rdur * rdur;
   float slope = 4.0 * amplitude * (rdur - rdur2);
   float curve = -8.0 * amplitude * rdur2;

   for(int i=0;i<length;i++){
      ep->data[i] = amp;
      amp = amp + slope;
      slope = slope + curve;
   }
   return ep;

}


evelope *evelope_gen_raised_cosine_bell(evelope* ep, int type, int length, float amplitude, int attacksamples, int releasesamples){
   ep = malloc(sizeof(evelope));
   ep->data = malloc(sizeof(float) * length);
   ep->type = type;
   ep->length = length;
   ep->attacksamples = attacksamples;
   ep->releasesamples = releasesamples;

   float amp = 0;

   for(int i=0;i<attacksamples;i++){
      amp = (1.0 + cos( PI + ( PI * ( i / (float) attacksamples )))) * (amplitude / 2.0);
      ep->data[i] = amp;
   }

   for(int i=attacksamples;i<length-releasesamples;i++){
      amp = amplitude;
      ep->data[i] = amp;
   }


   for(int i=length-releasesamples;i<length;i++){
      amp = (1.0 + cos( PI +  PI * ( i / (float) releasesamples ) ) ) * (amplitude / 2.0);
      ep->data[i] = amp;
   }

   return ep;
}


evelope *evelope_gen_no_evelope(evelope* ep, int type, int length, float amplitude){
   ep = malloc(sizeof(evelope));
   ep->data = malloc(sizeof(float) * length);
   ep->type = type;
   ep->length = length;
   ep->attacksamples = 0;
   ep->releasesamples = 0;

   for(int i=0;i<length;i++){
      ep->data[i] = amplitude;
   }
   return ep;
}


evelopbuf *evelopbuf_new(int size){

	evelopbuf *eb = malloc(sizeof(evelopbuf));
	if (!eb) return NULL;

   eb->data = malloc(sizeof(evelope) * size);
   eb->size = size;
   eb->front = 0;
   eb->rear = 0;
   post("evelopbuf newed!");

   return eb;
}


void evelopbuf_free(evelopbuf *eb){
	// free every evelopes in the buffer
   evelope* ep = NULL; // tmp grain saver
   while (evelopbuf_is_empty(eb) == 0){
      ep = evelopbuf_pop_evelope(eb, ep);   
      evelope_free(ep);
   }

    // free the buffer itself
   free(eb->data);
   free(eb);
   post("evelopbuf freed!");
}


evelope *evelopbuf_pop_evelope(evelopbuf *eb, evelope* ep){
   if (evelopbuf_is_empty(eb) == 1){
      return NULL;
   }
   ep = eb->data[eb->front]; 
      
   eb->data[eb->front] = NULL;

   eb->front = (eb->front + 1) % eb->size;
   return ep;
}


void evelopbuf_add_evelope(evelopbuf *eb, int type, int length){
   evelope* ep = NULL;

   if (evelopbuf_is_full(eb) == 1){
      ep = evelopbuf_pop_evelope(eb, ep);   // remove the earliest added evelop to move space for a new one
      evelope_free(ep);
      // eb->rear = (eb->rear+ 1) % eb->size;      
   }
   ep = evelope_new(ep, type, length, 0.99, 512, 512); // todo move this parameters to scheduler
   // ep = evelope_new(ep, type, length, amplitude, attacksamples, releasesamples);
   eb->data[eb->rear] = ep;     
   eb->rear = (eb->rear+ 1) % eb->size;
   post("front: %d | rear : %d", eb->front, eb->rear);
}



evelope *evelopbuf_check_evelope(evelopbuf *eb, int type, int length){
   evelope* ep = NULL;
   for (int i = 0; i < evelopbuf_get_len(eb); i++){
      ep = eb->data[(eb->front + i) % eb->size];
      if (type == ep->type && length == ep->length){ // todo: use a evelopbuf_compare_evelope() rather than this
         return ep;
      }
   }
   // if desired evelope not found, add it and return.
   evelopbuf_add_evelope(eb, type, length);
   ep = eb->data[(eb->rear - 1 + eb->size) % eb->size];
   return ep;
}


int evelopbuf_is_full(evelopbuf *eb){
   return (eb->rear+1)%eb->size == eb->front?1:0;
}


int evelopbuf_is_empty(evelopbuf *eb){
   return eb->rear == eb->front?1:0;
}


int evelopbuf_get_len(evelopbuf *eb){
   return (eb->rear - eb->front + eb->size)%eb->size;
}





         // post("statue of ep: \n type: %d | length: %d",ep->type, ep->length);
         // for(int i=0;i<length;i++){
         //    post("sample No.%d: %f", i, ep->data[i]);
         // }