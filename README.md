# LRUCache
A C++ generic  Least Recently Used Cache

In general, I write software in Java for server side code or JavaScript 
for client (browser) user interface code.  Over my career I have also written 
large software systems in C++.  This includes compilers for the hardware design 
languages Verilog and VHDL, sensor monitoring and control software and physics 
simulation code.

I had a consulting opportunity for software development work in C++, so I needed
something to refresh my C++ knowledge. To help in this endeavor, I bought a copy
of Bjarne Stroustrup's book "A Tour of C++". This book was the perfect reference
for brushing up on my C++.

After reading through Stroustrup's book, I needed something to write some code.
I decided to implement a generic version of an LRU cache as a C++ template.

LRU caches have a number of applications. They can serve as a front end for a
database, where the cache key is a database query. If the query is seen again
while the {key, value} pair are stored in the cache, the cached value can
be returned without referencing the database.

This implementation of the LRU cache uses the C++ standard template library (STL)
unordered map. This map support O(1) access.



