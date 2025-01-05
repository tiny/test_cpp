
#include <iostream>
#include <errno.h>
#include <string>
#include <stdint.h>

// assumption:  both strings are lowercase alpha, no numerics or untypables, and length < 16k
//
// return:
//    0  is an anagram
//   -1  null string / bad param
//   -2  not anagram / uneven lengths
//   -3  not anagram / unmatched letters
//
int16_t is_anagram(const std::string a, const std::string b)
{
  const char* aptr = a.c_str();
  const char* bptr = b.c_str();
  if ((aptr == nullptr) || (bptr == nullptr)) return -1;

  int16_t     arr[26];
  memset(arr, 0x00, sizeof(int16_t) * 26);

  while (*aptr && *bptr)
  {
    arr[*aptr++ - 97]++;
    arr[*bptr++ - 97]--;
  }

  if (*aptr != *bptr) return -2; // uneven lengths

  for (int i = 0; i < 26; i++)
    if (arr[i] != 0) return -3; // mismatched letter found

  return 0; // is an anagram
} // :: is_anagram

void test_anagram()
{
  std::string  s(NULL);
  if (is_anagram((const char*)NULL, "mutilate") == 0)
    printf("is anagram\n");
  else
    printf("not an anagram\n");

  if (is_anagram("ultimate", "mutilate") == 0)
    printf("is anagram\n");
  else
    printf("not an anagram\n");

  if (is_anagram("remote", "meteor") == 0)
    printf("is anagram\n");
  else
    printf("not an anagram\n");
} // ;: test_anagram

#include <vector>

template <typename T>
struct Comparable
{
public:
  bool operator!=(const Comparable<T>& other) {
    return !(*this == other);
  }

  bool operator<=(const Comparable<T>& other) {
    return ((*this == other) || (*this < other));
  }

  bool operator>(const Comparable<T>& other) {
    return !(*this == other) && !(*this < other);
  }

  bool operator>=(const Comparable<T>& other) {
    return !(*this < other);
  }
};

class Spotify : Comparable<Spotify>
{
  int data;
  friend bool operator==(Spotify const& lhs, Spotify const& rhs) {
    return lhs.data == rhs.data;
  }
  friend bool operator< (Spotify const& lhs, Spotify const& rhs) {
    return lhs.data < rhs.data;
  }
public:
  Spotify(int x) { data = x; }
};

void test_spotify()
{
  std::vector<Spotify>  v1;
  std::vector<Spotify>  v2;

  v1.push_back(5);
  v1.push_back(7);
  v2.push_back(5);
  v2.push_back(9);
  for (int i = 0; i < 2; i++)
  {
    if (v1[i] == v2[i]) {
      printf("good\n");
    }
    else {
      printf("xxx\n");
    }
  }
} // :: test_spotify

int foo() { return 2; }
int bar() { return 2; }
int baz(int x) { return 1; }

void test_cpp()
{
  int rc = foo();
  int x = 1;
  if (rc == 1)
    if (bar())
      baz(x);
    else if (rc == 2)
      baz(x);


  (x == 1) && foo();

  ((rc = foo()) == 1) ? bar() && baz(x) : (rc == 2) && baz(x);
} // ;: test_cpp

class Foo
{
private:
  int  _x;

public:
  Foo(int x) { _x = x; }

  virtual void doit() {
    // some work
    std::cout << "starting threaded work";
    for (int i = 0; i < 10; i++) {
      _x += 1;
      std::cout << " .";
    }
    std::cout << "\n";
  }
};

#include <thread>

void test_thread()
{
  Foo          f(5);
  std::thread  t(&Foo::doit, &f);

  t.join();
  std::cout << "done.\n";
} // test_thread

int main()
{
  //  printf("i prefer printf\n");

  //  std::cout << "I prefer cout" << std::endl;

  //  test_anagram();

  //  test_spotify();

    //test_cpp();

  test_thread();

  return 0;
}
