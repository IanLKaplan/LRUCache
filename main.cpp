
#include "Cache.hpp"
#include <string>
#include <iostream>

using namespace std;

struct Element {
    Element(string& strval, int v) : s(strval), val(v) {}
    ~Element() = default;
    const string s;
    const int val;
};

int main() {
    Cache<string, Element> cache(4);
    string str = "one";
    Element elemOne(str, 1);
    cache.put(str, elemOne);
    str = "two";
    Element elemTwo( str, 2);
    cache.put(str, elemTwo);
    str = "three";
    Element elemThree( str, 3);
    cache.put(str, elemThree);
    str = "four";
    Element elemFour( str, 4);
    cache.put(str, elemFour);
    str = "five";
    Element elemFive( str, 5);
    cache.put(str, elemFive);
    str = "two";
    cache.put(str, elemTwo);
    unique_ptr<vector<pair<string, Element>>> hashVals = cache.mapVector();
    int size = (int)hashVals->size();
    for (auto& elem : *hashVals) {
        cout << "key = " << elem.first << endl;
    }
    cout << "size = " << size;
    return 0;
}
