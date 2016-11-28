# COMP4133 Information Retrieval Project
A simple search engine written in C++.

# How to build
## Build on Linux
Install gcc and g++.
Clone the project from Github.
`make`

##Build on Windows
Install MinGW (mingw32-base, mingw32-gcc-g++, msys-base).
Clone the project from Github.
Run msys.
Move to the project directory.
`make`

# Testing
This program has been tested on Fedora 24.

# Run
Place _post1.txt_ into _data/data/_ directory.

```
Usage: comp4133 [-b | -n | -v] [-tf <tf function>] 
                [-idf <idf function>] [-nf <nf function> <slope>]
                queryfile
Options:

Select retrieval model
   -b        Boolean model
   -n        Boolean model(NLP)
   -v        Vector space model(VSM)

Select tf function (for VSM)
   -tf 1     Binary
   -tf 2     Raw frequency
   -tf 3     Log normalization
   -tf 4     Double normalization 0.5
   -tf 5     Double normalization 0.4 0.6 (default)

Select idf function (for VSM)
   -idf 1     Unary
   -idf 2     Inverse document frequency
   -idf 3     Inverse document frequency smooth
   -idf 4     Inverse document frequency max
   -idf 5     probabilistic inverse document frequency

Select normalization function (for VSM)
   -nf 1            Cosine Similarity - vector lengths (default)
   -nf 2 <slope>    Pivoted cosine normalization
   -nf 3 <slope>    Pivoted unique normalization
```
