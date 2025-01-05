/*
*  test_cpp.cpp
*  test program for c/c++
* 
*  developer   tiny
*  date        22 march 2024 
*/
#include <iostream>

using namespace std;

#ifdef DONT_USE

//-----------------------------------------------------------------------------
#include <Windows.h>
#include <profileapi.h>
#include <sysinfoapi.h>
#include <minwinbase.h>

class StopwatchReport
{
protected:
  std::string desc;
  double    timerFreq;
  uint64_t  freq;
  uint64_t  t1;
  uint64_t  t2;
  uint64_t  diff;

  void      begin() {
    nbytes = 0;
    t1 = now();
    t2 = t1;
  }
  uint64_t  now() {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    uint64_t tt = ft.dwHighDateTime;
    tt <<= 32;
    tt |= ft.dwLowDateTime;
    tt /= 10;
    //      tt -= 11644473600000000ULL;
    return tt;
  }
  void report() {
    double rate = 0.0;
    if (diff != 0) {
      rate = (double)nbytes / (double)diff;
      rate /= (1024.0 * 1024.0); // bytes to MB
      rate *= (1000.0 * 1000.0); // micro to sec
    }
    uint64_t  sec = diff / 1000000;
    uint64_t  usec = diff % 1000000;
    printf("%s: %02lld.%06lld sec \n", desc.c_str(), sec, usec );
  }
public:
  uint64_t  nbytes;
  std::string md5;
  StopwatchReport(const std::string& desc_) : desc(desc_) { begin(); }
  ~StopwatchReport() { end();  report(); }

  void      end() {
    if (t2 == t1) {
      t2 = now();
    }
    diff = t2 - t1;
  }
}; // class StopwatchReport

//-----------------------------------------------------------------------------
class Foo
{
  private:
    int  _x;
    int  _y;
    int  _z;

  protected:
  public:
    Foo();
    ~Foo();

    void calc();
    void print();
    int  set(int a);
} ; // class Foo

Foo::Foo()
{
  _x = 0;
  _y = 0;
  _z = 0;
  calc();
} // Foo [ctor]

Foo::~Foo()
{
} // Foo [dtor]

void Foo::calc()
{
  // business rules
  // x == literal; range 0..10
  // y == x * 2
  // z == y + 3
  //
  _y = _x * 2;
  _z = _y + 3;
} // Foo::calc

void Foo::print()
{
  std::cout << "x: " << _x;
  std::cout << " y: " << _y;
  std::cout << " z: " << _z;
  std::cout << std::endl;
} // Foo::print

int Foo::set(int a)
{
  if ((a < 0) || (a > 10))
  {
    return -1;
  }

  _x = a;
  calc();
  return 0;
} // Foo::set


//-----------------------------------------------------------------------------
void my_func()
{
  std::cout << "doing the func" << std::endl;
} // :: my_func

int main_01(int argc, const char* argv[]) 
{
  std::cout << "hello world" << std::endl;

  my_func();

  Foo  f;
  f.print();

  f.set(5);
  f.print();

  f.set(7);
  f.print();

  if (f.set(11) != 0)
  {
    std::cout << "Error.  x invalid (" << 11 << ")" << std::endl;
  }
  f.print();

  std::cout << "walking out" << std::endl;

  return 0;
} // :: main

//------------------------------------------------------------------------------------------
#include <iostream>
#include <thread>

void kid_1(void) {
  for (int i = 0; i < 10; ++i) {
    std::cout << "kid 1: Line " << i + 1 << " - C++" << std::endl;
  }
}

void kid_2(void) {
  for (int i = 0; i < 10; ++i) {
    std::cout << "kid 2: Line " << i + 1 << " - C++" << std::endl;
  }
}

int thread_test() {
  std::thread  t1(kid_1);
  std::thread  t2(kid_2);

  for (int i = 0; i < 10; ++i) {
    std::cout << "Parent Thread: Line " << i + 1 << " - C++" << std::endl;
  }

  t1.join();
  t2.join();
  return 0;
} // :: thread_test

//------------------------------------------------------------------------------------------
class Foo2
{
public:
  std::string  _name;

  Foo2();
  Foo2( const char *name );
//  Foo2( Foo2& c);
  ~Foo2();
  Foo2& operator= (const Foo2& c);
}; // class Foo2

Foo2::Foo2()
  : _name("unk")
{
  printf("%p  ctor.1   str( %p )\n", this, _name.c_str());
} // Foo2 [ctor]

Foo2::Foo2(const char* name)
  : _name( name )
{
  printf("%p  ctor.2   str( %p )\n", this, _name.c_str());
} // Foo2 [ctor]

//Foo2::Foo2( Foo2& c)
//{
//  printf("%p  ctor.3\n", this);
//  *this = c;
//} // Foo2 [ctor]

Foo2::~Foo2()
{
  printf("%p  dtor  str( %p )\n", this, _name.c_str() );
} // Foo2 [dtor]

Foo2& Foo2::operator= (const Foo2& c)
{
  printf("%p  copy\n", this);
  if (this != &c)
  {
    _name = c._name;
  }
  return *this;
} // Foo2::operator=


void test_ctor()
{
  Foo2  f("fred");

  {
    Foo2  x( f );
    printf("inner  str.1( %p )  str.2( %p )\n", f._name.c_str(), x._name.c_str());
  }
  printf("outer\n");
  printf("done\n");
} // :: test_ctor

//------------------------------------------------------------------------------------------
#include <map>
#include <vector>

void load_strings(const string &fname, vector<string>& a) 
{
  FILE* inf = nullptr;
  fopen_s( &inf, fname.c_str(), "rb");
  if (inf != nullptr) 
  {
    int n = 1;
    char  buf[1025];
    char  *ch;
    buf[1024] = '\0'; 

    while (n > 0)
    {
//      n = fscanf_s(inf, "%[^,],%[^\n]\n", id, buf);
      n = fscanf_s(inf, "%[^\r]\r\n", buf, 1024 );
      ch = strchr(buf, ',');
      if (ch) {
        a.push_back(string(&ch[1]));
      }
    }
    fclose(inf);
  }
} // :: load_strings

void test_overlap()
{
  vector<string>  x = { "Black Mirror", "Rocky", "IT", "The Thing", "Titanic", "Pretty Woman", "Stranger Things" };
  vector<string>  y = { "Independence Day", "Stranger Things", "The IT Crowd", "The Thing", "Disenchantment", "Black Mirror", "Frida", "Rudy" };

  {
    StopwatchReport  sw("loading");
    load_strings("movies.txt", y);
    sw.end();
  }

  {
    StopwatchReport  sw("mapped");

    vector<string>* a = (x.size() < y.size()) ? &x : &y;
    vector<string>* b = (x.size() < y.size()) ? &y : &x;
    map<string, int>  c ;
    vector<string>  d;

    for (auto &i : *a) c.insert(map<string, int>::value_type(i, 1));
  
    for (auto &i : *b) {
      auto it = c.find(i);
      if (it != c.end()) d.push_back(i);
    }

    sw.end();
    printf("#movies in common:  %d out of %d movies \n", d.size(), (a->size() + b->size() - d.size()));
  }
} // :: test_overlap

//----------------
struct X {
  X() { std::cout << "X"; }
};

struct Y {
  Y(const X& x) { std::cout << "Y"; }
  void f() { std::cout << "f"; }
};

void test_thing()
{
  Y y(X());
  y.f();
}

#endif

//------------------------------------------------------------------------------------------
#ifdef DONT_USE_TEST_FILE_PARSE

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

void my_read();

struct my_struct {
  my_struct* self;
  int x;
  int y;
  int z;
  void (*read)();

/*
  static my_struct* ctor(int w) {
    my_struct* s = malloc(sizeof(my_struct));

    s->self = s;
    s->x = w;
    s->read = my_read;
  }
*/
};

void my_read() {
  struct my_struct self;
  struct my_struct *s = &self;
  //= (my_struct*)new uint8_t[sizeof(my_struct)];
  int16_t  sz = sizeof(my_struct);

  printf("my_read.0:  %p\n", &self);

  for (int i = 0; i < 100; i++) {
    printf("Value: %p\n", &s[1*i]);
  }
}


void test_thang()
{
    
  struct my_struct thing;
  thing.x = 100;
  thing.y = 200;
  thing.z = 300;
  thing.read = my_read;

  printf("base ptr: %p\n", thing);

//  thing.read();
  my_read();
} 
//------------------------------------------------------------------------------------------
#ifdef _WIN64
#  define  byte  uint8_t
#  include <Windows.h>
#elif defined(LINUX)
#include <sys/time.h>
#endif

uint64_t now() 
{
#ifdef _WIN64
  LARGE_INTEGER frequency, counter;

  // Get the frequency of the high-resolution performance counter
  QueryPerformanceFrequency(&frequency);
  // Get the current value of the performance counter
  QueryPerformanceCounter(&counter);

  // Convert the counter value to microseconds
  return (counter.QuadPart * 1000000) / frequency.QuadPart;

#elif defined(LINUX) 
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
#endif
} // :: now

char* strdup(const char* src, int16_t sz)
{
  char* nu = (char*)malloc(sz + 1);
  if (nu)
  {
    memcpy(nu, src, sz);
    nu[sz] = '\0';
  }
  return nu;
} // :: strdup

char** realloc(char** subs, int16_t old_sz, int16_t new_sz)
{
  char **nu_subs = (char**)malloc( new_sz * sizeof(void*));
  if (nu_subs)
  {
    if (old_sz > 0) {
      memcpy(nu_subs, subs, old_sz * sizeof(void*));
    }
    memset(&nu_subs[old_sz], 0x00, (new_sz - old_sz) * sizeof(void*));
    free(subs);
  }
  return nu_subs;
} // :: realloc

char** strsplit2array(const char* str, char delim, int16_t* sz)
{
  char** subs = nullptr;
  int16_t  inc = 250;
  int16_t  ndx = 0;
  const char* ch;
  *sz = 0;

  while (*str)
  {
    ch = strchr(str, delim);
    if (ndx + 1 > *sz) 
    {
      subs = realloc(subs, *sz, *sz+inc);
      *sz += inc;
    }
    if (ch == nullptr) 
      ch = strchr(str, '\0');
    subs[ndx++] = strdup(str, (ch - str));
    str = (*ch == '\0') ? ch : &ch[1];
  }

  return subs;
} // :: strsplit2array

#ifdef FIRST_PASS

char* create_big_string(int32_t n)
{
  int32_t avg_chars = 6;
  int32_t  sz = ((n+1) * (avg_chars+1));
  char* buf = (char*)malloc(sz+1);
  if (buf == nullptr)
    return nullptr;

  char* ch = buf;
  for (int32_t i = 0; i < n; i++)
  {
    for (int32_t j = 0; j < avg_chars; j++)
      *ch++ = 'a' + (rand() % 26);
    *ch++ = ' ';
  }
  *ch = '\0';
  return buf;
} // :: create_big_string

#else

char* create_big_string(int32_t n)
{
  int32_t avg_chars = 6;
  int32_t  sz = ((n + 1) * (avg_chars + 1));
  char* buf = (char*)malloc(sz + 1);
  char* ch = buf;
  while (sz > 10) {
    for (int32_t i=0;i<6;i++) { *ch++ = 'a' + (rand() % 26); }
    *ch++ = ' ';
    sz -= 7;
  }
  *ch = '\0';
  return buf;
} // :: create_big_string

#endif

void test_splitstr()
{
  char** subs = nullptr;
  int16_t array_sz = 0;
//  const char* sample_str = "This a sample string that should have a number of substrings";
  char* sample_str = create_big_string(10000);
  char delim = ' ';

  uint64_t  t1 = now();

  subs = strsplit2array(sample_str, delim, &array_sz);

  uint64_t  t2 = now();
  printf("test_splitstr :  time to parse:  %6ld usec\n", (t2 - t1));

  // dump
//  printf("str [%s] \n", sample_str);
//  for (int i = 0; i < min( 100, array_sz); i++)
//  {
//    if (subs[i])
//    {
//      printf("%d:  [%s] \n", i, subs[i]);
//    }
//  }

  // cleanup
  for (int i = 0; i < array_sz; i++)
    free(subs[i]);
  free(subs);
  free(sample_str);
} // :: test_splitstr

//------------------------------------------------------------------------------------------
// 
// the performance of this is pretty bad compared to the one above
//
//
char** split_by_delimeter(char* string, char delim, int* arrlength)
{
  uint16_t d_count = 0; // number of deliminator instances
  for (int i = 0; i < strlen(string) - 1; i++) // excluding last element
    if (string[i] == delim)
      d_count += 1;

  char** array = (char**)calloc(d_count+2, sizeof(char*));
  int arr_index = 0;
  char* last_delim = &string[0];

  // iterating through all elements including null terminator at the end
  for (int string_index = 1; string_index <= strlen(string); string_index++)
  {
    if (string[string_index] == delim || string[string_index] == '\0')
    {
      size_t length = (&string[string_index] - last_delim);
      char* substr = array[arr_index];
      substr = (char*)calloc(length + 1, sizeof(char));
      memcpy(substr, last_delim, length);
      substr[length] = '\0';
      last_delim = &string[string_index + 1];
      arr_index++;
    }
  }

  *arrlength = arr_index;

  return array;
} // :: split_by_delimeter

void test_splitstr2()
{
  char** subs = nullptr;
  int array_sz = 0;
  //  const char* sample_str = "This a sample string that should have a number of substrings";
  char* sample_str = create_big_string(10000);
  char delim = ' ';

  uint64_t  t1 = now();

  subs = split_by_delimeter(sample_str, delim, &array_sz);

  uint64_t  t2 = now();
  printf("test_splitstr2:  time to parse:  %6ld usec\n", (t2 - t1));

  // dump
//  printf("str [%s] \n", sample_str);
//  for (int i = 0; i < min(100, array_sz); i++)
//  {
//    if (subs[i])
//    {
//      printf("%d:  [%s] \n", i, subs[i]);
//    }
//  }

  // cleanup
  for (int i = 0; i < array_sz; i++)
    if (subs[i])
      free(subs[i]);
  free(subs);
  free(sample_str);
} // :: test_splitstr2


//------------------------------------------------------------------------------------------
struct FileStruct01
{
  uint32_t  tag;
  uint32_t  b;
  uint32_t  c;
  uint32_t  d;
  uint32_t  e;
  uint32_t  cnt;
  uint32_t  g;
  uint32_t  h;

  uint32_t  unk[8];
};

struct FileStruct02
{
  uint32_t  unk;
  uint32_t  offset;
};

#define   MAX_FILE_READ   64*1024 

int16_t file_dump(FILE *inf, const char* fname, uint32_t pos, int32_t sz)
{
  FILE* outf = nullptr;
  errno_t err = fopen_s( &outf, fname, "wb+");
  char buf[MAX_FILE_READ];
  int32_t  amt = 0, n;

  if (outf)
  {
    fseek(inf, pos+4, SEEK_SET);
    sz -= 4;
    while (sz > 0)
    {
      amt = min(MAX_FILE_READ, sz);
      n = fread_s(buf, MAX_FILE_READ, 1, amt, inf);
      if (n < 0) break;
      fwrite(buf, 1, n, outf);
      sz -= n;
    }
    fclose(outf);
  }
  return 0;
} // :: file_dump

void test_file_parse(const char* fname)
{
  FILE* inf = nullptr;
  errno_t  err = fopen_s(&inf, fname, "rb");
  if (inf)
  {
    // read header
    FileStruct01   hdr;
    FileStruct02  *rec = nullptr;
    uint32_t       ndx_sz = 0;

    fread_s(&hdr, sizeof(hdr), sizeof(hdr), 1, inf);

    // allocate index
    ndx_sz = hdr.cnt * sizeof(FileStruct02);
    rec = new FileStruct02[hdr.cnt];

    // read directory
    fread_s(rec, ndx_sz, ndx_sz, 1, inf);

    // parse
    char buf[256];
    for (int i = 0; i < hdr.cnt; i++) 
    {
      sprintf_s(buf, 256, "output//file_%03d.bin", i);

      fseek(inf, rec[i].offset, SEEK_SET);
      file_dump(inf, buf, rec[i].offset, rec[i+1].offset - rec[i].offset);
    }
    fclose(inf);
    delete rec;
  }
} // :: test_file_parse

#endif // DONT_USE_TEST_FILE_PARSE

//------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <chrono>

void process_strings()
{
  printf("process string test (c++ version)\n");
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

  // Get the resolution of high_resolution_clock
  auto resolution = std::chrono::high_resolution_clock::period::num / 
                    static_cast<double>(std::chrono::high_resolution_clock::period::den);

  std::cout << "Resolution of high_resolution_clock: " << resolution << " seconds" << std::endl;

  printf( "time to produce random block: %ld ns \n", (p2-p1).count() ) ;
  printf( "time to produce random strings: %ld ns \n", (p3-p2).count() ) ;
  printf( "time to calculate average string length: %ld ns \n", (p4-p3).count() ) ;
  printf( "average length is: (%d)  %6.2f characters\n", cnt, (float)cnt / 1000.0 ) ;

} // :: process_strings

//------------------------------------------------------------------------------------------
int main()
{
//  thread_test();
//  test_ctor();
//  test_overlap();

//  test_thing();

//  test_thang();

//  test_splitstr ();
//  test_splitstr2();

//  test_file_parse( "pack0_core_xga.big" );

  process_strings() ;

  return 0;
} // :: main