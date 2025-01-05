
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <chrono>

void process_strings()
{
  srand(time(NULL)) ;

  char rand_buf[1024+1] ;
  char strings[1000][256] ;
  char *ch = rand_buf ;
  auto p1 = std::chrono::high_resolution_clock::now() ;

  // random character block
  for (int i = 0; i < 1024; i++) {
    *ch++ = 'A' + rand() % (int)('z' - 'A') ;
  }
  rand_buf[1024] = '\0' ;
  auto p2 = std::chrono::high_resolution_clock::now() ;

  // generate strings
  for (int i = 0; i < 1000; i++) {
    int16_t  n = (rand() % 100) + 50 ; // random length
    int16_t  p = (rand() % 512) + 256 ; // random starting point in the block
    memcpy( strings[i], &rand_buf[p], n ) ;
    strings[i][n] = '\0'; 
  }
  auto p3 = std::chrono::high_resolution_clock::now() ;

  // now find average length
  int32_t cnt = 0 ;
  for (int i = 0; i < 1000; i++) {
    cnt += strlen( strings[i] ) ;
  }
  auto p4 = std::chrono::high_resolution_clock::now() ;

  printf( "time to produce random block: %ld ns \n", (p2-p1).count() ) ;
  printf( "time to produce random strings: %ld ns \n", (p3-p2).count() ) ;
  printf( "time to calculate average string length: %ld ns \n", (p4-p3).count() ) ;
  printf( "average length is: (%d)  %6.2f characters\n", cnt, (float)cnt / 1000.0 ) ;

} // :: process_strings

int main()
{
  printf("process string test (c++ version)\n");
  process_strings();

  return 0;
}
