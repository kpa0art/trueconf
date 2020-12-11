// Example program
#include <iostream>

#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <functional>
#include <time.h> 
#include <stdlib.h>

using Map = std::map<int, int>;
using Vec = std::vector<int>;
using Set = std::unordered_set<int>;

template<class Pred>
void save_map_items_if(Map& m, Pred&& p) {
  for (Map::iterator i = m.begin(); i != m.end(); )
    if (p(*i))
      i++;
    else
      i = m.erase(i);

  Map tmp;
  std::copy_if(m.begin(), m.end(), std::inserter(tmp, tmp.end()), p );
  tmp.swap(m);
}

template<class Pred>
void save_vector_items_if(Vec& v, Pred&& p) {
  v.erase(
    std::remove_if( v.begin(), v.end(), [&](int e) { return !p(e); }), 
    v.end()
  );

  Vec tmp;
  std::copy_if(v.begin(), v.end(), std::inserter(tmp, tmp.end()), p );
  tmp.swap(v);
}

Set values_of_map(const Map& m) {
  Set s(m.size());
  for (const auto& kv : m)
    s.insert(kv.second);
  return s;
}

Set values_of_vector(const Vec& v) {
  return Set(v.begin(), v.end(), v.size());
}

void save_map_items_present_in_vector(Map& m, const Vec& v) {
  Set s = values_of_vector(v);
  save_map_items_if(m, [&s](const auto &kv) { return s.find(kv.second) != s.end(); });
}

void save_vector_items_present_in_map(Vec& v, const Map& m) {
  Set s = values_of_map(m);
  save_vector_items_if(v, [&s](int e) { return s.find(e) != s.end(); });
}

void synchronize(Map& m, Vec& v) {
  if (v.size() < m.size()) {
    save_map_items_present_in_vector(m, v);
    save_vector_items_present_in_map(v, m);
  } else {
    save_vector_items_present_in_map(v, m);
    save_map_items_present_in_vector(m, v);
  }
}

void print_vector(Vec &v) {
  std::cout << "Vec: ";
  if ( v.empty() )
    std::cout << "empty";
  else
    for (auto it: v)
      std::cout << it << " ";
  std::cout << std::endl;
}

void print_map(Map &m) {
  std::cout << "Map: ";
    
  if ( m.empty() )
    std::cout << "empty";
  else
    for (auto it: m)
      std::cout << it.second << " ";  
        
  std::cout << std::endl;
}

int get_random_number(int min, int max) {
    return min + (rand() % (max - min + 1));    
}

void randomize_map(Map &m, int size, int min, int max) {
  m.clear();
  
  for (int i = 0; i < size; i++)
    m[i] = get_random_number( min, max );
}

void randomize_vector(Vec &v, int size, int min, int max) {
  v.clear();
  
  for (int i = 0; i < size; i++)
    v.push_back( get_random_number( min, max ) );
}

void random_remove_from_map(Map &m, int nOfRemoving) {
    
  for ( int i = 0; i < nOfRemoving; i++ ) {
    auto iter = m.begin();
    std::advance(iter, get_random_number( 0, m.size() - 1 ));
    m.erase(iter);
  }
} 

void random_remove_from_vector(Vec &v, int nOfRemoving) {
  for ( int i = 0; i < nOfRemoving; i++ ) {
    auto iter = v.begin();
    std::advance(iter, get_random_number( 0, v.size() - 1 ));
    v.erase(iter);
  }    
}    
    

int main()
{
  Map m;
  Vec v;
  
  int size = 10, min = 1, max = 9;
  
  srand(time(0));
  
  randomize_map( m, size, min, max );
  randomize_vector( v, size, min, max );
  
  print_vector(v);
  print_map(m); 
  std::cout << std::endl;
  
  int nRemovedFromMap = size > 15 ? get_random_number(0, 14) : get_random_number(0, size - 1);
  int nRemovedFromVec = size > 15 ? get_random_number(0, 14) : get_random_number(0, size - 1);
  
  std::cout << "removed from map: "<< nRemovedFromMap << std::endl;
  std::cout << "removed from vector: "<< nRemovedFromVec << std::endl;
  std::cout << std::endl;
  
  random_remove_from_map( m, nRemovedFromMap );
  random_remove_from_vector( v, nRemovedFromVec);
  
  print_vector(v);
  print_map(m); 
  std::cout << std::endl;
    
  synchronize(m, v);
  
  std::cout << "synchronized" <<  std::endl;
    
  print_vector(v);
  print_map(m);
  std::cout << std::endl;
}
