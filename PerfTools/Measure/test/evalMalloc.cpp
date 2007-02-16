#include <bits/c++allocator.h>
#undef ___glibcxx_base_allocator

#include <ext/pool_allocator.h>
#define ___glibcxx_base_allocator  __gnu_cxx::__pool_alloc

/*
mallinfo returns a structure that describes the current status of the allocation arena. The structure has at least the following 
members:

   	size_t arena;     total arena size in bytes 
   	size_t ordblks;   number of full-sized allocations
   	size_t smblks;    number of small-sized allocations 
   	size_t hblks;     number of small-sized allocation containers 
   	size_t hblkhd;    overhead for these containers, in bytes 
   	size_t usmblks;   total bytes of in-use small-sized allocations 
   	size_t fsmblks;   total bytes of available small-sized allocations 
   	size_t uordblks;  total bytes of in-use full-sized allocations 
   	size_t fordblks;  total bytes of available full-sized allocations 
*/

/*
  int arena;     non-mmapped space allocated from system
  int ordblks;   number of free chunks
  int smblks;    number of fastbin blocks
  int hblks;     number of mmapped regions 
  int hblkhd;    space in mmapped regions
  int usmblks;   maximum total allocated space
  int fsmblks;   space available in freed fastbin blocks
  int uordblks;  total allocated space 
  int fordblks; /total free space 
  int keepcost; top-most, releasable (via malloc_trim) space 
*/

#include<malloc.h>
#include<iostream>
#include<vector>
#include<list>
#include<cstdlib>
void print(char const * title) {
	std::cout << "\n--- " << title <<" ---"<< std::endl;
	struct mallinfo mi;
	mi  = mallinfo();
	int * mm = (int*)(&mi);
	for(int i=0;i<10;i++) std::cout << mm[i] << ", ";
	std::cout << std::endl;
        std::cout << "mmap/arena-used/arena-free " << mi.hblkhd << " " << mi.uordblks << " " << mi.fordblks << std::endl;
        std::cout << "mmap/arena-used/arena-free " << mm[4] << " " << mm[7] << " " << mm[8] << std::endl;
        std::cout << std::endl;
        malloc_stats();
}

void printDiff(char const * title, 
	       struct mallinfo const & mi,
	       struct mallinfo const & mo) {
  int const * mmi = (int const*)(&mi);
  int const * mmo = (int const*)(&mo);
  std::cout << "\n" << title << ": mmap/arena-used/arena-free "
	    << mmo[4]-mmi[4] << " "
	    << mmo[7]-mmi[7] << " "
	    << mmo[8]-mmi[8] << " "
	    << std::endl;
}

struct PrintDiff {
  PrintDiff(char const * ititle) : title(ititle), mi(::mallinfo()) {
  }
  ~PrintDiff() {
    printDiff(title.c_str(),mi,::mallinfo());
  }

  std::string title;
  struct mallinfo mi;
};

int * a;
std::vector<int> gv;
std::list<int> gl;

int main() {
  if (::getenv("GLIBCXX_FORCE_NEW")) std::cout << "GLIBCXX_FORCE_NEW defined" << std::endl;       
  if (::getenv("GLIBCPP_FORCE_NEW")) std::cout << "GLIBCPP_FORCE_NEW defined" << std::endl;
  
  
  print("Start");
  
  {
    PrintDiff d("new 1000");
    a = new int[1000];
  }
  print("new 1000");

  {
    PrintDiff d("deleted");
    delete [] a;
  }
  print("deleted");

  {
    PrintDiff d("new 1M");
    a = new int[1000000];
  }
  print("new 1M");
  
  {
    PrintDiff d ("deleted");
    delete [] a;
  }
  print("deleted");
  
  {
    std::vector<int> v;
    {
      PrintDiff d ("vector 20K");
      v.resize(20000);
      v.swap(gv);
    }
    print("vector 20K");
    v.swap(gv);
  }

  {
    std::list<int> v;
    {
      PrintDiff d ("list 20K");
      v.resize(20000);
      v.swap(gl);
    }
    print("list 20 K");
    v.swap(gl);
  }       

  {
    std::vector<int> v;
    {
      PrintDiff d ("vector 2M");
      v.resize(2000000);
      v.swap(gv);
    }
    print("vector 2M");
    v.swap(gv); 
  }
  { 
    std::list<int> v;
   {
      PrintDiff d ("list 2M");
      v.resize(2000000);
      v.swap(gl);
   }
   print("list 2M");
   v.swap(gl);
  }
  
  print("before clear");
  
  {
    PrintDiff d ("all gone");
    gl.clear();
    gv.clear();
    { std::vector<int> v; v.swap(gv); }
  }
  
  print("all gone");
  
  {
    PrintDiff d ("trim to 0");
    malloc_trim(0);
  }
  print("trim to 0");
  
  if (::getenv("GLIBCXX_FORCE_NEW")) std::cout << "GLIBCXX_FORCE_NEW defined" << std::endl;
  if (::getenv("GLIBCPP_FORCE_NEW")) std::cout << "GLIBCPP_FORCE_NEW defined" << std::endl;
  
  return 0;
}
